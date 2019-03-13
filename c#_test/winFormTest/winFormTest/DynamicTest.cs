using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Xml;
namespace winFormTest
{
    public partial class DynamicTest : Form
    {
        generate m_rUpdateHelper;
        XmlDocument m_rDoc = new XmlDocument();
        public DynamicTest()
        {
            InitializeComponent();
            m_rUpdateHelper = new generate(ref this.tableLayoutPanel1);
        }

        public void updateView()
        {
            string szContent = readXml("");
            m_rDoc.LoadXml(szContent);

            m_rUpdateHelper.rebuild(szContent);
        }
        public void updateView(string szName)
        {
//             string szContent = readXml("test.xml");
//             m_rDoc.LoadXml(szContent);
            //XmlNodeList rList = m_rDoc.SelectNodes("//after");
            m_rUpdateHelper.rebuild(szName);
        }
        private string readXml(string szPath)
        {
            StreamReader rRead = new StreamReader(szPath, Encoding.GetEncoding("GB2312"));
            string szContent = rRead.ReadToEnd();
            rRead.Close();
            return szContent;
        }
    }
}
