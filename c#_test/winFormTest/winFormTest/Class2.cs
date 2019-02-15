using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.IO;

namespace winFormTest
{
    class xmlOperator
    {
        public static void CreateXML(string xmlName,ref System.Windows.Forms.TextBox rOutput)
        {
            //通过代码创建XML文档
            //1、引用命名空间   System.Xml
            //2、创建一个 xml 文档
            XmlDocument xml = new XmlDocument();
            //3、创建一行声明信息，并添加到 xml 文档顶部
            xml.Load(xmlName);

            readChild(xml,ref rOutput, 0);

        }
        public static void outPutNewFile(string xmlName)
        {
            XmlDocument xml = new XmlDocument();
            //xml.Load(xmlName);
            string szCOntent = FormatXml(xmlName);
            StreamWriter rFile = new StreamWriter("szCOntent.xml");
            rFile.Write(szCOntent);
            rFile.Close();
            //xml.Save(XmlWriter.Create("testXml.xml"));

        }
        private static string FormatXml(string sUnformattedXml)
        {
            XmlDocument xd = new XmlDocument();
            xd.Load(sUnformattedXml);
            StringBuilder sb = new StringBuilder();
            StringWriter sw = new StringWriter(sb);
            XmlTextWriter xtw = null;
            try
            {
                xtw = new XmlTextWriter(sw);
                //xtw.Formatting = Formatting.Indented;
                xtw.Indentation = 1;
                xtw.IndentChar = '\t';
                xd.WriteTo(xtw);
            }
            finally
            {
                if (xtw != null)
                    xtw.Close();
            }
            return sb.ToString();
        }
        public static void readXmlToGrid(string xmlName,ref Dictionary<string, exchangeType> rDic)
        {
            //通过代码创建XML文档
            //1、引用命名空间   System.Xml
            //2、创建一个 xml 文档
            XmlDocument xml = new XmlDocument();
            //3、创建一行声明信息，并添加到 xml 文档顶部
            xml.Load(xmlName);
            var temp = xml.ChildNodes;
            for (int i = 0; i < temp.Count; i++)
            {
                if(temp[i].Name == "SeriesClothsetDynamic")
                {
                    readChildGrid(temp[i], ref rDic);
                }
            }
        }
        private static void readChildGrid(XmlNode xmlNode, ref Dictionary<string, exchangeType> rDic)
        {
            var x = xmlNode.ChildNodes;
            if (x.Count <= 0)
            {
                return ;
            }
            for (int i = 0; i < x.Count; i++)
            {
                if ("Clothset" == x[i].Name)
                {
                    if(null != x[i].Attributes && null != x[i].Attributes["set_id"])
                    {
                        string rID = x[i].Attributes["set_id"].Value;
                        rDic.Add(rID, handleExchangeType(x[i]));
                    }
                }
            }
        }
        private static exchangeType handleExchangeType(XmlNode rNode)
        {
            if(null != rNode.Attributes)
            {
                var offer_cast = rNode.Attributes["offer_enable"];
                if(null == offer_cast)
                {
                    var rNodeChildren = rNode.ChildNodes;
                    if(rNodeChildren == null)
                    {
                        return exchangeType.exchange_Close;
                    }
                    for(int index = 0; index < rNodeChildren.Count; ++index)
                    {
                        if(rNodeChildren[index].Attributes != null && rNodeChildren[index].Attributes["cost"] != null )
                        {
                            if(rNodeChildren[index].Attributes["cost"].Value != "-1")
                            {
                                return exchangeType.exchange_normal;
                            }
                            else
                            {
                                return exchangeType.exchange_Close;
                            }
                        }
                    }
                }
                else
                {
                    return exchangeType.exchange_First_Sale;
                }
            }
            return exchangeType.exchange_Close;
        }
        private static bool readChild(XmlNode xmlDoc, ref System.Windows.Forms.TextBox rOutput,int level)
        {
            var x = xmlDoc.ChildNodes;
            if(x.Count <= 0)
            {
                return false;
            }
            for (int i = 0; i < x.Count; i++)
            {
                for(int j = 0; j < level;++j)
                {
                    rOutput.AppendText(" "); 
                }
                if("#comment" == x[i].Name)
                {
                    rOutput.AppendText("type:" + x[i].Name + " ");
                    rOutput.AppendText("value: " + x[i].Value);
                }
                else
                {
                    rOutput.AppendText("type:" + x[i].Name);
                    if(null == x[i].Attributes)
                    {
                        continue;
                    }
                    for (int index = 0; index < x[i].Attributes.Count; ++index)
                    {
                        rOutput.AppendText("key: " + x[i].Attributes[index].Name);
                        rOutput.AppendText("value: " + x[i].Attributes[index].Value);
                    }
                }
                rOutput.AppendText("\n");
                readChild(x[i],ref rOutput, level + 1);
            }
            return true;
        }
        public static void appendXml(string xmlName)
        {
            //创XML建对象
            XmlDocument doc = new XmlDocument();
            //声明根节点
            XmlElement books;
            //判断文件是否存在
            if (File.Exists(xmlName))
            {
                //该文件存在
                //加载文件
                doc.Load(xmlName);
                //获得根节点
                books = doc.DocumentElement;

            }
            else//该文件不存在
            {
                //创建声明
                XmlDeclaration dec = doc.CreateXmlDeclaration("1.0", "utf-8", null);
                doc.AppendChild(dec);
                //创建根节点
                books = doc.CreateElement("Books");
                doc.AppendChild(books);
            }

            //开始正常写入信息就可以了
            XmlElement book1 = doc.CreateElement("Book");
            books.AppendChild(book1);

            XmlElement name = doc.CreateElement("Name");
            name.InnerText = "大话西游" + (new Random()).Next(0, 1000);
            book1.AppendChild(name);

            doc.Save("Books.xml");
            Console.WriteLine("追加完成");
            Console.ReadKey();
        }
    }
    
}
