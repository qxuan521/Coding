using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.IO;

namespace X51Tools.Global
{
    class XmlOperator
    {
        //brief
        //xml的操作类基类封装，提供添加、修改、查询等普通接口，特别提供
        //
        //property
        private XmlDocument m_rXmlDoc;
        //function
        //
        //打开Xml文件
        //
        public void openXml(string xmlName)
        {
            m_rXmlDoc = new XmlDocument();
            m_rXmlDoc.Load(xmlName);
        }

        public XmlDocument getDoc()
        {
            return m_rXmlDoc;
        }

    }
}
