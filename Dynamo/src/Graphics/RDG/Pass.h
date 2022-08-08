#pragma once

class Graphics;
class In;
class Out;

class Pass {
public:
	Pass(const std::string& name);
	virtual ~Pass();
	virtual void Run(Graphics& g) = 0;
	virtual void Clear();
	virtual void Finish();

	inline const std::string& Name() const { return m_Name; }
	inline std::vector<Unique<In>>& Ins() { return m_Ins; }
	inline std::vector<Unique<Out>>& Outs() { return m_Outs; }

	void Link(const std::string& in, const std::string& target);
	In& GetIn(const std::string& name) const;
	Out& GetOut(const std::string& name) const;

protected:
	void AddIn(Unique<In> in);
	void AddOut(Unique<Out> out);

private:
	std::string m_Name;
	std::vector<Unique<In>> m_Ins;
	std::vector<Unique<Out>> m_Outs;
};

