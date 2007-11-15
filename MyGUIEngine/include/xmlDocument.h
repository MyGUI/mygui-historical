#ifndef _XMLDOCUMENT_H_
#define _XMLDOCUMENT_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "assert.h"


namespace xml
{

	namespace utility
	{
		template< class T >
		inline std::string toString (T p)
		{
			std::ostringstream stream;
			stream << p;
			return stream.str();
		}

		template< class T1, class T2 >
		inline std::string toString (T1 p1, T2 p2)
		{
			std::ostringstream stream;
			stream << p1 << p2;
			return stream.str();
		}

		inline void trim(std::string& _str, bool _left = true, bool _right = true)
		{
			if (_right) _str.erase(_str.find_last_not_of(" \t\r")+1);
			if (_left) _str.erase(0, _str.find_first_not_of(" \t\r"));
		}

	}

	typedef enum xmlNodeType {
		XML_NODE_TYPE_REMARK, // ����������
		XML_NODE_TYPE_INFO, // �������������� ����
		XML_NODE_TYPE_NORMAL, // ������������ ����
	};

	namespace errors
	{

		typedef enum ErrorTypes {
			XML_ERROR_NONE,
			XML_ERROR_OPEN_FILE,
			XML_ERROR_CREATE_FILE,
			XML_ERROR_BODY_NON_CORRECT,
			XML_ERROR_NON_CLOSE_ALL_TAGS,
			XML_ERROR_DOCUMENT_IS_EMPTY,
			XML_ERROR_CLOSE_TAG_NOT_FOUND_START_TAG,
			XML_ERROR_OPEN_CLOSE_NOT_EQVIVALENT,
			XML_ERROR_INFO_IS_EXIST,
			XML_ERROR_ROOT_IS_EXIST,
			XML_ERROR_ATTRIBUTE_NON_CORRECT,
			XML_ERROR_COUNT
		};

	} // namespace errors

	class xmlNode;
	class xmlDocument;

	typedef xmlNode * xmlNodePtr;
	typedef std::pair<std::string, std::string> PairAttributes;
	typedef std::vector<PairAttributes> VectorAttributes;
	typedef std::vector<xmlNodePtr> VectorNode;

	//----------------------------------------------------------------------//
	// class xmlNodeIterator
	//----------------------------------------------------------------------//
	class xmlNodeIterator
	{
		friend xmlNode;

	private:
		xmlNodeIterator(VectorNode::iterator _start, VectorNode::iterator _end);

	public:
		bool nextNode();
		bool nextNode(const std::string & _name);

		inline xmlNodePtr operator->() const {assert(m_current != m_end); return (*m_current);}

	private:
		bool m_first;
		VectorNode::iterator m_current, m_end;
	};


	//----------------------------------------------------------------------//
	// class xmlNode
	//----------------------------------------------------------------------//
	class xmlNode
	{
		friend  xmlDocument;

	public:
		~xmlNode();

	private:
		xmlNode(const std::string &_name, xmlNodePtr _parent, xmlNodeType _type = XML_NODE_TYPE_NORMAL, const std::string & _body = "");

		void save(std::ofstream & _stream, size_t _level);

	public:
		xmlNodePtr createChild(const std::string & _name, const std::string & _body = "");

		inline void addAttributes(const std::string &_key, const std::string &_value) {m_attributes.push_back(PairAttributes(_key, _value));}
		inline void addAttributes(const std::string &_key, int _value) {m_attributes.push_back(PairAttributes(_key, utility::toString(_value)));}
		inline void addAttributes(const std::string &_key, size_t _value) {m_attributes.push_back(PairAttributes(_key, utility::toString(_value)));}
		inline void addAttributes(const std::string &_key, float _value) {m_attributes.push_back(PairAttributes(_key, utility::toString(_value)));}

		inline void addBody(const std::string &_body) {m_body.empty() ? m_body = utility::toString(_body) : m_body += utility::toString(" ", _body);}
		inline void addBody(int _body) {m_body.empty() ? m_body = utility::toString(_body) : m_body += utility::toString(" ", _body);}
		inline void addBody(size_t _body) {m_body.empty() ? m_body = utility::toString(_body) : m_body += utility::toString(" ", _body);}
		inline void addBody(float _body) {m_body.empty() ? m_body = utility::toString(_body) : m_body += utility::toString(" ", _body);}

		void clear();

		bool findAttribute(const std::string & _name, std::string & _value);

		inline const std::string & getName() {return m_name;}
		inline const std::string & getBody() {return m_body;}
		inline const VectorAttributes & getAttributes() {return m_attributes;}
		inline xmlNodePtr getParent() {return m_parent;}

		inline xmlNodeIterator getNodeIterator() {return xmlNodeIterator(m_childs.begin(), m_childs.end());}

	private:
		std::string m_name;
		std::string m_body;
		VectorAttributes m_attributes;
		VectorNode m_childs;
		xmlNodePtr m_parent;
		xmlNodeType m_type;
	};

	//----------------------------------------------------------------------//
	// class xmlDocument
	//----------------------------------------------------------------------//
	class xmlDocument
	{
	public:
		xmlDocument();
		~xmlDocument();

		bool open(const std::string & _name);
		bool open(const std::wstring & _name);

		// ��������� ����
		bool save(const std::string & _name);
		bool save(const std::wstring & _name);

		void clear();
		const std::string getLastError();

	private:

		bool open(std::ifstream & _stream);

		bool parseTag(xmlNodePtr &_currentNode, std::string _body);

		bool checkPair(std::string &_key, std::string &_value);

		// ���� ������ ��� ����� �������
		size_t find(const std::string & _text, char _char, size_t _start = 0);

		void clearInfo();
		void clearRoot();

	public:
		xmlNodePtr createInfo(const std::string & _version = "1.0", const std::string & _encoding = "UTF-8");
		xmlNodePtr createRoot(const std::string & _name);

		inline xmlNodePtr getRoot() {return m_root;}

	private:
		xmlNodePtr m_root;
		xmlNodePtr m_info;
		xml::errors::ErrorTypes m_lastError;
		size_t m_line;
		size_t m_col;

	}; // class xmlDocument : public xmlNode

} // namespace xml

#endif // _XMLDOCUMENT_H_
