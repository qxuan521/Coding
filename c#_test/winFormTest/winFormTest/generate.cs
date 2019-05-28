using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Xml;

namespace winFormTest
{
    public class generate
    {
        private TableLayoutPanel m_rPanel = null;
        List<PropertyContral> m_rTableContralList = new List<PropertyContral>();
        private XmlDocument m_rDoc = new XmlDocument();

        public generate(ref TableLayoutPanel rTablePanel)
        {
            m_rPanel = rTablePanel;
            string szContent = readXml("test.xml");
            m_rDoc.LoadXml(szContent);
        }

        public void rebuild(string xml)
        {
            if (m_rPanel != null)
            {
                XmlNodeList rList = m_rDoc.SelectNodes("//Step");
                for (int index = 0; index < rList.Count; ++index)
                {
                    if(rList[index].Attributes["name"]!= null && rList[index].Attributes["name"].Value == xml)
                    {
                        m_rPanel.Controls.Clear();
                        m_rPanel.RowCount = 0;
                        buildHelper(rList[index]);
                        return;
                    }
                }

            }
        }


        private string readXml(string szPath)
        {
            StreamReader rRead = new StreamReader(szPath, Encoding.GetEncoding("GB2312"));
            string szContent = rRead.ReadToEnd();
            rRead.Close();
            return szContent;
        }


        private void buildHelper(XmlNode rNode)
        {
            XmlNodeList rList = rNode.SelectNodes("Parm"); 
            for (int index = 0; index < rList.Count; ++index)
            {
                if (m_rPanel.RowCount == 0)
                {
                    m_rPanel.RowCount = 2;
                    this.m_rPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
                    this.m_rPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
                }
                else
                {
                    m_rPanel.RowCount = m_rPanel.RowCount + 1;
                    m_rPanel.RowStyles.Insert(m_rPanel.RowCount - 2, new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
                }
                PropertyContral rContral = PropertyFactory.CreateProperty(rList[index].Attributes["Type"].Value);
                m_rPanel.Controls.Add((Control)rContral, 1, m_rPanel.RowCount - 2);
                Label rTag = new Label();
                rTag.Dock = DockStyle.Fill;
                rTag.Margin = new Padding(0, 10, 0, 5);
                rTag.Text = rList[index].Attributes["Name"].Value;
                m_rPanel.Controls.Add(rTag, 0, m_rPanel.RowCount - 2);
                rContral.setValue(rList[index].Attributes["Value"].Value);
                rContral.getControl().Dock = DockStyle.Fill;
            }
        }
    }

    static class PropertyFactory
    {
        public static PropertyContral CreateProperty(string szPropertyXml)
        {
            switch (szPropertyXml)
            {
                case "float":
                    return new FloatNumberProperty();
                case "int":
                    return new IntNumberProperty();
                case "string":
                    return new StringProperty();
                default:
                    return null;
            }
        }
    }
}
