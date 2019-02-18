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
            StreamReader rRead = new StreamReader(xmlName, Encoding.GetEncoding("GB2312"));
            string szContent = rRead.ReadToEnd();
            
            //rOutput.AppendText(szContent);
            xml.Load(xmlName);
            //xml.LoadXml(szContent);
            //readChild(xml,ref rOutput, 0);
            StringBuilder rStringBuilder = new StringBuilder();
            writeHead(xml, ref rStringBuilder);
            StreamWriter rFile = new StreamWriter("szCOntent.xml", false, Encoding.GetEncoding("GB2312"));
            string szResult = rStringBuilder.ToString();
            rFile.Write(rStringBuilder.ToString());
            rFile.Close();
        }
        private static void writeHead(XmlNode xmlDoc, ref StringBuilder rOutput)
        {
            var x = xmlDoc.ChildNodes;
            if (x.Count <= 0)
            {
                return;
            }
            for (int i = 0; i < x.Count; i++)
            {
                if(x[i].Name == "SeriesClothsetDynamic")
                {
                    rOutput.Append("<SeriesClothsetDynamic>\r\n");
                    writeChild(x[i], ref rOutput,1,true);
                    rOutput.Append("</SeriesClothsetDynamic>");
                }
                else
                {
                    rOutput.Append(x[i].OuterXml);
                    rOutput.AppendLine("");
                }
            }
        }
        public static void outPutNewFile(string xmlName)
        {
            XmlDocument xml = new XmlDocument();
           // xml.Load(xmlName);
            string szCOntent = FormatXml(xmlName);
            StreamWriter rFile = new StreamWriter("szCOntent.xml", false,Encoding.GetEncoding("GB2312"));
            rFile.Write(szCOntent);
            rFile.Close();
            //xml.Save("szCOntent.xml");

        }
        private static string FormatXml(string sUnformattedXml)
        {
            XmlDocument xd = new XmlDocument();
            xd.Load(sUnformattedXml);
            StringBuilder sb = new StringBuilder();
            StringWriter sw = new StringWriter(sb);
            XmlTextWriter xtw = null;
            //string szResult = 
            try
            {
                xtw = new XmlTextWriter(sw);
                xtw.Formatting = Formatting.Indented;
                xtw.Indentation = 1;
                xtw.IndentChar = '\t';
                //xtw = new XmlTextWriter("szCOntent.xml", Encoding.GetEncoding("GB2312"));
                xd.WriteTo(xtw);
            }
            finally
            {
                if (xtw != null)
                    xtw.Close();
            }
            return sb.ToString();
        }
        //private static string FormatXml(string sUnformattedXml)
        //{
        //    XmlDocument xd = new XmlDocument();
        //    xd.Load(sUnformattedXml);
        //    StringBuilder sb = new StringBuilder();
        //    StringWriter sw = new StringWriter(sb);
        //    XmlTextWriter xtw = null;
        //    try
        //    {
        //        xtw = new XmlTextWriter(sw);
        //        xtw.Formatting = Formatting.Indented;
        //        xtw.Indentation = 1;
        //        xtw.IndentChar = '\t';
        //        //xtw = new XmlTextWriter("szCOntent.xml", Encoding.GetEncoding("GB2312"));
        //        xd.WriteTo(xtw);
        //    }
        //    finally
        //    {
        //        if (xtw != null)
        //            xtw.Close();
        //    }
        //    return sb.ToString();
        //}
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
                if(x[i].Value != null)
                {
                    rOutput.AppendText("node value: " + x[i].Value);
                }
                else
                {
                    rOutput.AppendText("node value: null");
                }
                if ("#comment" == x[i].Name)
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
        private static string getNodePre(XmlNode xmlNode)
        {
            StringBuilder rBuilder = new StringBuilder();
            rBuilder.Append("<");
            rBuilder.Append(xmlNode.Name);
            if(xmlNode.Attributes != null)
            {
                for(int index = 0; index < xmlNode.Attributes.Count;++index)
                {
                    rBuilder.Append(" " + xmlNode.Attributes[index].Name);
                    rBuilder.Append("=\"" + xmlNode.Attributes[index].Value + "\"");
                }
            }
            rBuilder.Append(">");
            return rBuilder.ToString();
        }
        private static string getNodeEnd(XmlNode xmlNode)
        {
            StringBuilder rBuilder = new StringBuilder();
            rBuilder.Append("</");
            rBuilder.Append(xmlNode.Name);
            rBuilder.Append(">");
            return rBuilder.ToString();
        }
        private static void writeChild(XmlNode xmlDoc, ref StringBuilder rOutput, int level,bool isFirstSpace)
        {
            var x = xmlDoc.ChildNodes;
            if (x.Count <= 0)
            {
                return;
            }
           
            for (int i = 0; i < x.Count; i++)
            {
                if(x[i].HasChildNodes)
                {//has child
                    if(!(i == 0 && !isFirstSpace))
                    {
                        AppendSpace(ref rOutput, level);
                    }
                    if(specialNode(x[i]))
                    {
                        rOutput.Append(getNodePre(x[i]));
                        writeChild(x[i], ref rOutput, level + 1, false);
                    }
                    else
                    {
                        rOutput.Append(getNodePre(x[i]) + "\r\n");
                        writeChild(x[i], ref rOutput, level + 1, true);
                    }
                    AppendSpace(ref rOutput, level);
                    rOutput.Append(getNodeEnd(x[i]) + "\r\n");
                }
                else
                {
                    if (!(i == 0 && !isFirstSpace))
                    {
                        AppendSpace(ref rOutput, level);
                    }
                    rOutput.Append(x[i].OuterXml);
                    if ("text" == x[i].Name)
                    {
                        if (i + 1 < x.Count &&
                            "#comment" == x[i + 1].Name)
                        {
                            rOutput.Append(x[i + 1].OuterXml);
                            rOutput.AppendLine("");
                            //rOutput.Append("\r\n");
                            ++i;
                            continue;
                        }
                    }
                    rOutput.AppendLine("");
                }
            }
        }
        private static bool specialNode(XmlNode rNodex)
        {
            return ("text" == rNodex.Name || "ExtraRewards" == rNodex.Name);
        }
        private static void AppendSpace(ref StringBuilder rBuilder, int level)
        {
            for (int j = 0; j < level; ++j)
            {
                rBuilder.Append("  ");
            }
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
