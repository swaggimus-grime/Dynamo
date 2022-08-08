#include "dynamopch.h"
#include "Pass.h"

#include "Graphics.h"
#include "In.h"
#include "Out.h"

Pass::Pass(const std::string& name)
	:m_Name(std::move(name))
{
}

Pass::~Pass()
{
}

void Pass::Clear()
{
}

void Pass::Link(const std::string& in, const std::string& target)
{
	auto& i = GetIn(in);
	auto tg = Parse(target, '.');
	DYNAMO_ASSERT(tg.size() == 2, "Invalid format for target: " + target);
	i.Target(std::move(tg[0]), std::move(tg[1]));
}

In& Pass::GetIn(const std::string& name) const
{
	for (auto& in : m_Ins)
	{
		if (in->Name() == name)
		{
			return *in;
		}
	}

	std::ostringstream oss;
	oss << "In named [" << name << "] not found in pass: " << Name();
	throw DYNAMO_EXCEP(oss.str());
}

Out& Pass::GetOut(const std::string& name) const
{
	for (auto& out : m_Outs)
	{
		if (out->Name() == name)
		{
			return *out;
		}
	}

	std::ostringstream oss;
	oss << "Out named [" << name << "] not found in pass: " << Name();
	throw DYNAMO_EXCEP(oss.str());
}

void Pass::Finish()
{
	for (auto& in : m_Ins)
		in->ValidateLink();
	for (auto& out : m_Outs)
		out->ValidateLink();
}

void Pass::AddIn(Unique<In> in)
{
	if (!m_Ins.empty())
		std::for_each(m_Ins.begin(), m_Ins.end(),
			[&in](const auto& i) { DYNAMO_ASSERT(i->Name() != in->Name(), "In with name " + in->Name() + " already exists");  });
	
	m_Ins.push_back(std::move(in));
}

void Pass::AddOut(Unique<Out> out)
{
	if (!m_Outs.empty())
		std::for_each(m_Outs.begin(), m_Outs.end(),
			[&out](const auto& o) { DYNAMO_ASSERT(o->Name() != out->Name(), "Out with name " + out->Name() + " already exists");  });

	m_Outs.push_back(std::move(out));
}
