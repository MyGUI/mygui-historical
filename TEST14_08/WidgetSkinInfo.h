#pragma once

#include <vector>
#include <Ogre.h>
#include "BasisWidgetBinding.h"


namespace widget
{
	// ��������������� ����� ��� ������������� ������ �����
	class WidgetSkinInfo
	{

	public:
		WidgetSkinInfo()
		{
			checkState("normal");
		}

		void setInfo(int _cx, int _cy, const Ogre::String &_material, const Ogre::String &_font="", Ogre::ushort _fontHeight=0)
		{
			m_cx = _cx;
			m_cy = _cy;
			m_material = _material;
			m_font = _font;
			m_fontHeight = _fontHeight;
		}

		void addInfo(const BasisWidgetBinding & _bind)
		{
			checkState(_bind.m_states);
			m_basis.push_back(tagBasisWidgetInfo(_bind.m_type, _bind.m_offset, _bind.m_aligin));
			checkBasis();
			fillState(_bind.m_states, m_basis.size()-1);
		}

	private:
		void checkState(const ViewInfo & _states)
		{
			for (ViewInfo::const_iterator iter = _states.begin(); iter != _states.end(); iter ++) {
				checkState(iter->first);
			}
		}

		inline void checkState(const Ogre::String & _name)
		{
			// ���� ����� �� ����
			StateInfo::const_iterator iter = m_states.find(_name);
			if (iter == m_states.end()) {
				// ��������� ����� �����
				m_states[_name] = tagWidgetStateInfo();
			}
		}

		inline void checkBasis()
		{
			// � ����������� ������ �������� �� ����������� �����������
			for (StateInfo::iterator iter = m_states.begin(); iter!=m_states.end(); iter++) {
				while (iter->second.m_offsets.size() < m_basis.size()) {
					iter->second.m_offsets.push_back(Ogre::FloatRect(0, 0, 1, 1));
				}
			}
		}

		inline void fillState(const ViewInfo & _states, size_t _index)
		{
			for (ViewInfo::const_iterator iter = _states.begin(); iter != _states.end(); iter ++) {
				m_states[iter->first].m_offsets[_index] = iter->second.offset;
			}
		}

	public:
		inline int width() const {return m_cx;};
		inline int height() const {return m_cy;};
		inline const Ogre::String & getMaterial() const {return m_material;};
		inline const Ogre::String & getFontName() const {return m_font;};
		inline Ogre::ushort getFontHeight() const {return m_fontHeight;};
		inline const BasisInfo & getBasisInfo() const {return m_basis;};
		inline const StateInfo & getStateInfo() const {return m_states;};

	private:
		int m_cx, m_cy;
		Ogre::String m_material;
		Ogre::String m_font;
		Ogre::ushort m_fontHeight;
		BasisInfo m_basis;
		StateInfo m_states;

	};

} // namespace widget