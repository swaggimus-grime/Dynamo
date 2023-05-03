#pragma once

#include <functional>
#include "In.h"
#include "Out.h"
#include "Pass.h"
#include <imgui_node_editor.h>
namespace ed = ax::NodeEditor;

class RDG {
public:
	RDG(class Graphics& g);
	~RDG();
	void Run(Graphics& g);
	void Clear();
	void ShowGUI(Graphics& g);
	class RenderPass* GetRenderPass(const std::string& passName) const;

	inline ID3D11RenderTargetView* RTTarget() const {
		if (m_BackBuff) {
			auto& rt = dynamic_cast<ReadableRenderTarget&>(*m_BackBuff);
			return rt.GetTarget();
		}

		return nullptr;
	}

	inline ID3D11Texture2D* RTTextureFrFr() const {
		if (m_BackBuff) {
			auto& rt = dynamic_cast<ReadableRenderTarget&>(*m_BackBuff);
			return rt.GetTextureFrFr();
		}

		return nullptr;
	}

	inline ID3D11ShaderResourceView* RTTexture() const { 
		if (m_BackBuff) {
			auto& rt = dynamic_cast<ReadableRenderTarget&>(*m_BackBuff);
			return rt.GetTexture();
		}

		return nullptr;
	}

	inline ID3D11ShaderResourceView* DSTexture() const {
		if (m_MasterDS) {
			auto& ds = dynamic_cast<ReadableDepthStencil&>(*m_MasterDS);
			return ds.GetTexture();
		}

		return nullptr;
	}

	inline ID3D11ShaderResourceView* ShadowMap() const {
		if (m_ShadowMap) {
			auto& ds = dynamic_cast<ReadableDepthStencil&>(*m_ShadowMap);
			return ds.GetTexture();
		}

		return nullptr;
	}
protected:
	inline void AddGlobalIn(Unique<In> in);
	inline void AddGlobalOut(Unique<Out> out);
	void AddPass(Unique<Pass> pass);
	void LinkPass(Pass& pass);
	void Target(const std::string& in, const std::string& target);
	Pass& FindPassByName(const std::string& name);
	void Finish();
	void LinkGlobalIns();

	Shared<DepthStencil> m_ShadowMap;

private:
	std::vector<Unique<Pass>> m_Passes;
	std::vector<Unique<In>> m_Ins;
	std::vector<Unique<Out>> m_Outs;
	Shared<class RenderTarget> m_BackBuff;
	Shared<class DepthStencil> m_MasterDS;
	bool m_Finished = false;
	UINT m_OutNode;
	UINT m_InNode;
	Pass* m_SelectedPass;

	ed::EditorContext* m_Editor;
};

