#pragma once

#include <vector>
#include "../TinyXml/tinyxml.h"
#include "hgtypes.h"
#include "Export.h"


namespace hg3d {
    class CXmlNode;
    typedef std::vector<CXmlNode*> XmlNodeVec;

    class HG3D_EXPORT CXmlNode
    {
    public:
        typedef CXmlNode MyType;
        typedef CXmlNode NodeType;
        typedef std::vector<CXmlNode*> NodeSet;

        CXmlNode(void);
        CXmlNode(const CXmlNode&);
        virtual ~CXmlNode(void);

        static MyType* NewElementNode(const hg3d::TString& strNodeName);

        //
        CXmlNode& operator=(const CXmlNode& src);

        // overwrite
        virtual bool Parse(TinyXml::TiXmlNode* pNode);
        virtual bool Parse(const TCHAR* pszString);
        virtual TinyXml::TiXmlNode* ToNode();


        // attribute
        const tstring& GetAttributeValue(const TCHAR* pszName) const;
        const tstring& GetName() const;
        const XmlNodeVec& GetChildNodes() const;
        const bool IsTextNode() const;
        StringStringMap& GetAttributes() { return m_mapAttrs; }
        
        bool SetAttributeValue(const TCHAR* pszName, const TCHAR* pszValue);
        bool SetAttributeValue(const tstring& strName, const tstring& strValue);
        bool AddAttribute(const TCHAR* pszName, const TCHAR* pszValue);
        bool AddAttribute(const tstring& strName, const tstring& strValue);
        void SetName(const TCHAR* pszName);
        void SetName(const tstring& strName);


        MyType* GetParent(void) const { return m_parent; }

        /**
        * @brief ���ص�һ������ָ�����Ե��ӽڵ�
        * @param pszAttributeName ��������
        * @param pszAttributeValue ����ֵ
        * @retval NULL ���������Ľڵ㲻����
        * @retval !NULL �ӽڵ�ָ��
        */
        const NodeType* GetChildByAttribute(const TCHAR* pszAttributeName, const TCHAR* pszAttributeValue) const;
        NodeType* GetChildByAttribute(const TCHAR* pszAttributeName, const TCHAR* pszAttributeValue);

        /**
        * @brief ����·��ָ���Ľڵ�
        * @param pszPath �ӵ�ǰ�ڵ㿪ʼ�ľ���·�����硰/users/user/name"
        * @retval NULL ·��ָ���Ľڵ㲻����
        * @retval !NULL ·��ָ���Ľڵ�ָ��
        */
        CXmlNode* SelectChild(const TCHAR* pszPath);

        /**
        * @brief ����ָ��·���������ӽڵ�
        * @param pszPath �ӵ�ǰ�ڵ㿪ʼ�ľ���·�����硰/users"
        * @return �ӽڵ㼯�ϵ�����
        */
        NodeSet& SelectChildren(const TCHAR* pszPath);

        /**
        * @brief ����ָ��·����ƥ��ָ�����Ե������ӽڵ�
        * @param pszPath �ӵ�ǰ�ڵ㿪ʼ�ľ���·�����硰/users"
        * @param pszAttribute �ڵ����Ե�����
        * @param pszAttributeValue �ڵ����Ե�ֵ
        * @return �ӽڵ㼯��
        */
        XmlNodeVec SelectChildren(const TCHAR* pszPath, const TCHAR* pszAttribute, const TCHAR* pszAttributeValue);

        /**
        * @breif ����ӽڵ�
        * @param pNode �ӽڵ�ָ��
        * @retval true �ɹ�
        * @retval false ʧ��
        */
        bool AddChild(CXmlNode *pNode);

        /**
        * @breif ɾ���ӽڵ�
        * @param attributeName �ӽڵ�������
        * @param attributeValue �ӽڵ�����ֵ
        * @param recursive �Ƿ�ݹ�����ӽڵ�
        * @retval true �ɹ�
        * @retval false ʧ��
        */
        bool DeleteChildWidthAttribute(const TCHAR* attributeName, const TCHAR* attributeValue, bool recursive=false);
        bool RemoveChild(MyType* pChild);

        /**
        * @breif �޸����нڵ�����ֵ
        * @param attributeName �ڵ�������
        * @param valueOld ���Ծ�ֵ
        * @param valueNew ������ֵ
        * @retval ���޸ĵĽڵ���
        */
        int AlterAttributeRecursively(const hg3d::TString& attributeName,
            const hg3d::TString& valueOld, const hg3d::TString& valueNew);

    protected:
        TinyXml::TiXmlNode* ToXmlNode(void);
        TinyXml::TiXmlText* ToTextNode(void);
        TinyXml::TiXmlElement* ToElementNode(void);
        void AlterAttributeRecursively(const hg3d::TString& attributeName,
                                       const hg3d::TString& valueOld,
                                       const hg3d::TString& valueNew,
                                       int& count);
    protected:
        int             m_type;         ///< �ڵ�����
        TString         m_strName;      ///< �ڵ����ƣ������Text�ڵ����ǽڵ�Text
        StringStringMap m_mapAttrs;     ///< �ڵ����Լ���
        NodeSet         m_children;     ///< �ӽڵ㼯��
        MyType*         m_parent;       ///< ���ڵ�
    };
}
