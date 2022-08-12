#include "dynamopch.h"
#include "RDG.h"

#include "Graphics.h"
#include "RenderPass.h"
#include "Bindable/DepthStencil.h"
#include "Bindable/RenderTarget.h"
#include "Pass.h"

struct Link {
	ed::LinkId id;
	ed::PinId in;
	ed::PinId out;
};

std::vector<Link> links;
int linkID = 100;

RDG::RDG(Graphics& g)
	:m_BackBuff(MakeShared<ReadableRenderTarget>(g, g.Width(), g.Height(), 0)), m_MasterDS(MakeShared<WriteOnlyDepthStencil>(g)),
	m_Editor(ed::CreateEditor())
{
	AddGlobalOut(BufferOut<RenderTarget>::Make("backbuffer", m_BackBuff));
	AddGlobalOut(BufferOut<DepthStencil>::Make("masterDepth", m_MasterDS));
	AddGlobalIn(BufferIn<RenderTarget>::Make("backbuffer", m_BackBuff));
}

RDG::~RDG()
{
	ed::DestroyEditor(m_Editor);
}

void RDG::Run(Graphics& g)
{
	DYNAMO_ASSERT(m_Finished, "RDG not finished setting up");
	for (auto& p : m_Passes)
		p->Run(g);
}

void RDG::Clear()
{
	DYNAMO_ASSERT(m_Finished, "RDG not finished setting up");
	for (auto& p : m_Passes)
		p->Clear();
}

void RDG::ShowGUI()
{
	ImGui::Begin("Render Dependency Graph");
	ed::SetCurrentEditor(m_Editor);

	ed::Begin("RDG");
	int uniqueId = 500;

	ed::BeginNode(uniqueId++);
	ImGui::Text("RDG Globals");
	ImGui::BeginGroup();
	for (auto& in : m_Ins) {
		ed::BeginPin(in->PinID(), ed::PinKind::Input);
		ImGui::Text(("-> " + in->Name()).c_str());
		ed::EndPin();
	}
	ImGui::EndGroup();
	ImGui::SameLine();
	ImGui::BeginGroup();
	for (auto& out : m_Outs) {
		ed::BeginPin(out->PinID(), ed::PinKind::Output);
		ImGui::Text((out->Name() + " ->").c_str());
		ed::EndPin();
	}
	ImGui::EndGroup();
	ed::EndNode();

	for (auto& pass : m_Passes) {
		ed::BeginNode(uniqueId++);
		ImGui::Text(pass->Name().c_str());
		ImGui::BeginGroup();
		for (auto& in : pass->Ins()) {
			ed::BeginPin(in->PinID(), ed::PinKind::Input);
			ImGui::Text(("-> " + in->Name()).c_str());
			ed::EndPin();
		}
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
		for (auto& out : pass->Outs()) {
			ed::BeginPin(out->PinID(), ed::PinKind::Output);
			ImGui::Text((out->Name() + " ->").c_str());
			ed::EndPin();
		}
		ImGui::EndGroup();
		ed::EndNode();
	}

	for (auto& l : links)
		ed::Link(l.id, l.in, l.out);
	
	if (ed::BeginCreate())
	{
		ed::PinId inputPinId, outputPinId;
		if (ed::QueryNewLink(&inputPinId, &outputPinId))
		{
			if (inputPinId && outputPinId) // both are valid, let's accept link
			{
				if (ed::AcceptNewItem())
				{
					Link l{ linkID++, inputPinId, outputPinId };
					ed::Link(linkID++, l.in, l.out);
					links.push_back(std::move(l));
				}
			}
		}
	}
	ed::EndCreate();

	if (ed::BeginDelete())
	{
		// There may be many links marked for deletion, let's loop over them.
		ed::LinkId deletedLinkId;
		while (ed::QueryDeletedLink(&deletedLinkId))
		{
			// If you agree that link can be deleted, accept deletion.
			if (ed::AcceptDeletedItem())
			{
				// Then remove link from your data.
				for (int i = 0; i < links.size(); i++)
				{
					if (links[i].id == deletedLinkId)
					{
						auto& del = links.erase(links.begin() + i);
						break;
					}
				}
			}

			// You may reject link deletion by calling:
			// ed::RejectDeletedItem();
		}
	}
	ed::EndDelete();

	ed::End();
	ed::SetCurrentEditor(nullptr);
	ImGui::End();
}

RenderPass* RDG::GetRenderPass(const std::string& passName) const
{
	auto pass = std::find_if(m_Passes.begin(), m_Passes.end(), [&passName](const auto& p) { return passName == p->Name(); });
	DYNAMO_ASSERT(pass != m_Passes.end(), "Pass with name " + passName + " does not exist in RDG");
	return dynamic_cast<RenderPass*>(pass->get());
}

inline void RDG::AddGlobalIn(Unique<In> in)
{
	m_Ins.push_back(std::move(in));
}

inline void RDG::AddGlobalOut(Unique<Out> out)
{
	m_Outs.push_back(std::move(out));
}

void RDG::AddPass(Unique<Pass> pass)
{
	DYNAMO_ASSERT(!m_Finished, "RDG already finished setting up");
	for (const auto& p : m_Passes)
	{
		if (pass->Name() == p->Name())
		{
			throw DYNAMO_EXCEP("Pass name already exists: " + pass->Name());
		}
	}
	LinkPass(*pass);
	m_Passes.push_back(std::move(pass));
}

void RDG::LinkGlobalIns()
{
	for (auto& sink : m_Ins)
	{
		const auto& inputSourcePassName = sink->PassName();
		for (auto& existingPass : m_Passes)
		{
			if (existingPass->Name() == inputSourcePassName)
			{
				auto& source = existingPass->GetOut(sink->OutName());
				links.push_back(Link{ ed::LinkId(linkID++), ed::PinId(source.PinID()), ed::PinId(sink->PinID()) });
				sink->Link(source);
				break;
			}
		}
	}
}

void RDG::LinkPass(Pass& pass)
{
	for (auto& si : pass.Ins())
	{
		const auto& inputSourcePassName = si->PassName();

		// check check whether target source is global
		if (inputSourcePassName == "#")
		{
			bool bound = false;
			for (auto& source : m_Outs)
			{
				if (source->Name() == si->OutName())
				{
					links.push_back(Link{ed::LinkId(linkID++), ed::PinId(source->PinID()), ed::PinId(si->PinID()) });
					si->Link(*source);
					bound = true;
					break;
				}
			}
			if (!bound)
			{
				std::ostringstream oss;
				oss << "Output named [" << si->OutName() << "] not found in globals";
				throw DYNAMO_EXCEP(oss.str());
			}
		}
		else // find source from within existing passes
		{
			bool linked = false;
			for (auto& existingPass : m_Passes)
			{
				if (existingPass->Name() == inputSourcePassName)
				{
					auto& source = existingPass->GetOut(si->OutName());
					links.push_back(Link{ ed::LinkId(linkID++), ed::PinId(source.PinID()),  ed::PinId(si->PinID()) });
					si->Link(source);
					linked = true;
					break;
				}
			}
			if (!linked)
			{
				std::ostringstream oss;
				oss << "Pass named [" << inputSourcePassName << "] not found";
				throw DYNAMO_EXCEP(oss.str());
			}
		}
	}
}

//void RDG::Link(const std::vector<Unique<Out>>& outs, Unique<In>& in)
//{
//	auto& out = std::find_if(outs.begin(), outs.end(), [&in](const auto& o) { return o->Name() == in->OutName(); });
//	DYNAMO_ASSERT(out != outs.end(), "Cannot link an Out pointer with given name");
//	in->Link(*(*out));
//}

void RDG::Target(const std::string& in, const std::string& target)
{
	const auto finder = [&in](const auto& i) {
		return i->Name() == in;
	};

	const auto i = std::find_if(m_Ins.begin(), m_Ins.end(), finder);
	DYNAMO_ASSERT(i != m_Ins.end(), "Global In does not exist: " + in);

	auto tg = Parse(target, '.');
	DYNAMO_ASSERT(tg.size() == 2, "Target has invalid format");
	(*i)->Target(tg[0], tg[1]);
}

Pass& RDG::FindPassByName(const std::string& name)
{
	const auto i = std::find_if(m_Passes.begin(), m_Passes.end(), [&name](auto& p) {
		return p->Name() == name;
		});
	if (i == m_Passes.end())
	{
		throw std::runtime_error{ "Failed to find pass name" };
	}
	return **i;
}

void RDG::Finish()
{
	DYNAMO_ASSERT(!m_Finished, "RDG already finished setting up");
	for (const auto& p : m_Passes)
	{
		p->Finish();
	}
	LinkGlobalIns();
	m_Finished = true;
}
