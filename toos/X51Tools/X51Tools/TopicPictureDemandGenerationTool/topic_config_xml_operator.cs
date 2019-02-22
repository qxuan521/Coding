using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.IO;
using X51Tools.Global;


namespace X51Tools.TopicPictureDemandGenerationTool 
{
    class TopicConfigXmlOperator : X51Tools.Global.XmlOperator
    {
        //brief
        //配置文件操作类
        //property
        Dictionary<int, int> m_rMatherIDLast;
        XmlNode m_rPieceObtainNode = null;
        XmlNode m_rClothsetObtainText = null;
        XmlNode m_rRoot = null;
        XmlNode m_rLastClothsetNode = null;

        private delegate bool ForeachOperator(XmlNode rNode);

        public TopicConfigXmlOperator()
        {
            m_rMatherIDLast = new Dictionary<int, int>();
        }

        public void build(string szPath)
        {
            openXml(szPath);
            m_rRoot = getDoc();
            m_rLastClothsetNode = null;
            calcMatherIDMap();
        }
        //
        //获得子ID
        //
        public int getSubNextID(string szMatherID)
        {
            int nMather = int.Parse(szMatherID) / 100000;
            if (m_rMatherIDLast.ContainsKey(nMather))
            {
                return m_rMatherIDLast[nMather];
            }
            return -1;
        }
        //
        //获得碎片途径ID
        //
        public int getPieceObtainID()
        {
            return int.Parse(m_rPieceObtainNode.LastChild.Value);
        }
        //
        //部件获取途径ID
        //
        public int getClothsetObtainID()
        {
            return int.Parse(m_rClothsetObtainText.LastChild.Attributes["obtain_id"].Value);
        }
        //private
        //
        //计算套装各个母系列的最后一个子ID
        //
        private void calcMatherIDMap()
        {
            ForeachOperator rOperator = new ForeachOperator((XmlNode rNode) => 
            {
                if (rNode.Name == "Clothset")
                {
                    m_rLastClothsetNode = rNode;
                    if (null!= rNode.Attributes)
                    {
                        int nMather = int.Parse(rNode.Attributes["set_id"].Value) / 100000;
                        int nSubID = int.Parse(rNode.Attributes["set_id"].Value);
                        if(m_rMatherIDLast.ContainsKey(nMather))
                        {
                            int nOldID = m_rMatherIDLast[nMather];
                            m_rMatherIDLast[nMather] = nOldID < nSubID ? nSubID : nOldID;
                        }
                        else
                        {
                            m_rMatherIDLast.Add(nMather, nSubID);
                        }
                        return false;
                    }
                }
                else
                {
                    if (rNode.Name == "PieceObtainText")
                    {
                        m_rPieceObtainNode = rNode;
                    }
                    else if (rNode.Name == "ClothsetObtainText")
                    {
                        m_rClothsetObtainText = rNode;
                    }
                }
                return true;
            });
            foreachNode(rOperator);
        }
        //
        //追加新子系列
        //
        private void appendClothset(string[] rRowData, int rSourceID)
        {
            if(null == m_rLastClothsetNode)
            {
                return;
            }
            if(m_rLastClothsetNode.Attributes["set_name"].Value != rRowData[4])
            {
                XmlElement subElement = getDoc().CreateElement("Clothset");
                subElement.SetAttribute("series_id", rRowData[0]);
                subElement.SetAttribute("set_id", rRowData[1]);
                subElement.SetAttribute("set_name", rRowData[4]);
                subElement.SetAttribute("score_factor", rRowData[14]);
                subElement.SetAttribute("complete_score", rRowData[3]);
                subElement.SetAttribute("on_shelf_time", rRowData[2]);
                XmlElement node_1 = getDoc().CreateElement("#comment");
                node_1.Value = "Part 部件配置\r\nmale_id 男性玩家ID\r\nfemale_id 女性玩家ID\r\ncost 兑换部件需要的碎片个数\r\nscore 收集部件需要的分数，需支持小数配置\r\n";
                subElement.AppendChild(node_1);
                m_rRoot.InsertBefore(subElement, m_rLastClothsetNode);
                m_rLastClothsetNode = subElement;
            }
            XmlElement partElement = getDoc().CreateElement("Part");
            partElement.SetAttribute("male_id", rRowData[5]);
            partElement.SetAttribute("female_id", rRowData[7]);
            partElement.SetAttribute("cost", rRowData[10]);
            partElement.SetAttribute("score", rRowData[13]);
            m_rLastClothsetNode.AppendChild(partElement);
            XmlElement partElement_1 = getDoc().CreateElement("#comment");
            if(rRowData[6] != rRowData[8])
            {
                partElement_1.Value = rRowData[6] + "/" + rRowData[8];
            }
            else
            {
                partElement_1.Value = rRowData[6];
            }
            partElement.AppendChild(partElement_1);
            XmlElement partElement_2 = getDoc().CreateElement("obtain");//<obtain id = "1">
            partElement_2.SetAttribute("id", "1");
            partElement.AppendChild(partElement_2);

            XmlElement partElement_3 = getDoc().CreateElement("#comment");
            partElement_3.Value = "<!--Obtain  部件获取途径配置    id  获取途径的ID-->";
            partElement.AppendChild(partElement_3);

            XmlElement partElement_4 = getDoc().CreateElement("#comment");
            partElement_4.SetAttribute("id", rSourceID.ToString());
            partElement.AppendChild(partElement_4);

        }
        //
        //通用节点遍历方法
        //
        private void foreachNode(ForeachOperator rForeachOperator)
        {
            if(null == m_rRoot)
            {
                return;
            }
            foreachNodeHelper(m_rRoot, rForeachOperator);
        }

        private void foreachNodeHelper(XmlNode rNode, ForeachOperator rForeachOperator)
        {
            var x = rNode.ChildNodes;
            if (x.Count <= 0)
            {
                return;
            }
            for (int i = 0; i < x.Count; i++)
            {
                if(rForeachOperator(x[i]))
                {
                    foreachNodeHelper(x[i], rForeachOperator);
                }
            }
        }
    }
}
