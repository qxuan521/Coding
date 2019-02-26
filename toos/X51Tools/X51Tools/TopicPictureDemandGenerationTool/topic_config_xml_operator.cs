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
    public delegate void errorHandle(string szError);
    class TopicConfigXmlOperator : X51Tools.Global.XmlOperator
    {
        //brief
        //配置文件操作类
        //property
        Dictionary<int, int> m_rMatherIDLast;
        XmlNode m_rRoot = null;
        XmlNode m_rLastClothsetNode = null;
        XmlNode m_rLastRewardsNode = null;
        XmlNode m_rPartSouceParent = null;
        XmlNode m_rPieceSourceParent = null;

        Dictionary<string, string> m_rPartsSourceExsitMap;
        Dictionary<string, string> m_rPiecesSourceExsitMap;
        Dictionary<string,XmlNode> m_rClothsetMap;


        private delegate bool ForeachOperator(XmlNode rNode);

        public TopicConfigXmlOperator()
        {
            m_rMatherIDLast = new Dictionary<int, int>();
            m_rPartsSourceExsitMap = new Dictionary<string, string>();
            m_rPiecesSourceExsitMap = new Dictionary<string, string>();
            m_rClothsetMap = new Dictionary<string, XmlNode>();
        }

        public void build(string szPath)
        {
            openXml(szPath);
            var rChild = getDoc().ChildNodes;
            for (int index = 0; index < rChild.Count; ++ index)
            {
                if(rChild[index].Name == "SeriesClothsetDynamic")
                {
                    m_rRoot = rChild[index];
                }
            }
            m_rLastClothsetNode = null;
            m_rLastRewardsNode = null;
            m_rPartSouceParent = null;
            m_rPieceSourceParent = null;
            m_rPiecesSourceExsitMap.Clear();
            m_rPartsSourceExsitMap.Clear();
            m_rClothsetMap.Clear();
            calcMatherIDMap();
        }

        //
        //输出新文件
        //
        public void outputNewXml(string szPath)
        {
            StringBuilder rStringBuilder = new StringBuilder();
            fomatXml(ref rStringBuilder);
            StreamWriter rFile = new StreamWriter(szPath, false, Encoding.GetEncoding("GB2312"));
            string szResult = rStringBuilder.ToString();
            rFile.Write(rStringBuilder.ToString());
            rFile.Close();
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
            return int.Parse(m_rPieceSourceParent.LastChild.Value);
        }
        //
        //部件获取途径ID
        //
        public int getClothsetObtainID()
        {
            return int.Parse(m_rPartSouceParent.LastChild.Attributes["obtain_id"].Value);
        }
        //
        //追加新子系列
        //
        public void appendClothset(string[] rRowData, int rSourceID, string szCoverStr)
        {
            if (null == m_rLastClothsetNode)
            {
                return;
            }
            if (m_rLastClothsetNode.Attributes["set_name"].Value != rRowData[4])
            {
                XmlElement subElement = getDoc().CreateElement("Clothset");
                subElement.SetAttribute("series_id", rRowData[0]);
                subElement.SetAttribute("set_id", rRowData[1]);
                subElement.SetAttribute("set_name", rRowData[4]);
                subElement.SetAttribute("cover", szCoverStr);
                subElement.SetAttribute("score_factor", rRowData[14]);
                subElement.SetAttribute("complete_score", rRowData[3]);
                subElement.SetAttribute("on_shelf_time", rRowData[2]);
                string szNodeValue = "Part 部件配置\r\n      male_id 男性玩家ID\r\n      female_id 女性玩家ID\r\n      cost 兑换部件需要的碎片个数\r\n      score 收集部件需要的分数，需支持小数配置\r\n    ";
                XmlComment node_1 = getDoc().CreateComment(szNodeValue);
                subElement.AppendChild(node_1);
                m_rRoot.InsertBefore(subElement, m_rLastClothsetNode);
                m_rLastClothsetNode = subElement;
            }
            XmlElement partElement = getDoc().CreateElement("Part");
            partElement.SetAttribute("male_id", rRowData[6]);
            partElement.SetAttribute("female_id", rRowData[8]);
            partElement.SetAttribute("cost", rRowData[10]);
            partElement.SetAttribute("score", rRowData[13]);
            m_rLastClothsetNode.AppendChild(partElement);
            XmlComment partElement_1 = getDoc().CreateComment("");
            if (rRowData[5] != rRowData[7])
            {
                partElement_1.Value = rRowData[5] + "/" + rRowData[7];
            }
            else
            {
                partElement_1.Value = rRowData[5];
            }
            partElement.AppendChild(partElement_1);
            XmlElement partElement_2 = getDoc().CreateElement("Obtain");//<obtain id = "1">
            partElement_2.SetAttribute("id", "1");
            partElement.AppendChild(partElement_2);

            XmlComment partElement_3 = getDoc().CreateComment("Obtain  部件获取途径配置    id  获取途径的ID");
            partElement.AppendChild(partElement_3);

            XmlElement partElement_4 = getDoc().CreateElement("Obtain");
            partElement_4.SetAttribute("id", rSourceID.ToString());
            partElement.AppendChild(partElement_4);

        }
        //
        //追加获取途径
        //
        public void appendSource(string[] rRowData, errorHandle rErrorFunc)
        {
            if (null != m_rPartSouceParent)
            {
                if (m_rPartsSourceExsitMap.ContainsKey(rRowData[2]))
                {
                    rErrorFunc("error: " + rRowData[2] + "has exist");
                }
                else
                {
                    XmlElement partElement = getDoc().CreateElement("text");
                    m_rPartSouceParent.AppendChild(partElement);
                    partElement.SetAttribute("obtain_id", rRowData[2]);
                    partElement.SetAttribute("context", rRowData[3]);
                }
            }
            if (null != this.m_rPieceSourceParent)
            {
                if (m_rPiecesSourceExsitMap.ContainsKey(rRowData[1]))
                {
                    rErrorFunc("error: " + rRowData[1] + "has exist");

                }
                else
                {
                    XmlElement partElement = getDoc().CreateElement("text");
                    m_rPieceSourceParent.AppendChild(partElement);
                    partElement.SetAttribute("context", rRowData[1]);
                    string szSourceID = rRowData[0].Substring("碎片获取途径".Length);
                    XmlComment commentElement = getDoc().CreateComment(szSourceID);
                    m_rPieceSourceParent.AppendChild(commentElement);
                }
            }
        }

        // 
        //追加额外奖励 
        // 
        public void appendExtraReward(string[] rRowData)
        {
            if (null == m_rLastRewardsNode)
            {
                return;
            }
            XmlElement subElement = getDoc().CreateElement("ExtraRewards");
            int nID = int.Parse(m_rLastRewardsNode.Attributes["rewards_id"].Value);
            subElement.SetAttribute("rewards_id", (nID + 1).ToString());
            XmlComment node_1 = getDoc().CreateComment(rRowData[0]);
            subElement.AppendChild(node_1);
            m_rRoot.InsertBefore(subElement, m_rLastRewardsNode);
            m_rLastRewardsNode = subElement;
            //
            XmlElement rRewardItemsNode = getDoc().CreateElement("RewardItems");
            subElement.AppendChild(rRewardItemsNode);
            XmlElement rRewardItemNode = getDoc().CreateElement("RewardItem");
            //处理type的字符串
            int rNoValidIndex = rRowData[1].IndexOf('(');
            string szTypestring = string.Empty;
            if (rNoValidIndex != -1)
            {
                szTypestring = rRowData[1].Substring(rNoValidIndex);
            }
            else
            {
                szTypestring = rRowData[1];
            }
            rRewardItemNode.SetAttribute("type", szTypestring);
            rRewardItemNode.SetAttribute("para1", rRowData[3]);
            rRewardItemNode.SetAttribute("para2", rRowData[4]);
            rRewardItemNode.SetAttribute("para3", getTimeLimitHelper(rRowData[5]));
            //
            rRewardItemsNode.AppendChild(rRewardItemNode);
        }
        //
        //获取子系列的节点是否存在
        //
        public bool isClothsetExist(string szID)
        {
            return m_rClothsetMap.ContainsKey(szID);
        }
        
        //
        //获取节点优惠状态
        //
        public ExchangeState getClothSaleState(string szID, Dictionary<string,bool> rNoSaleID)
        {
           XmlNode rClothNode = m_rClothsetMap[szID];
           if(isCloseExchange(rClothNode, rNoSaleID))
           {
                return ExchangeState.Close;
           }
           else if(isSaleExchange(rClothNode,rNoSaleID))
            {
                return ExchangeState.Sale;
            }
            else if(isNormalExchange(rClothNode, rNoSaleID))
            {
                return ExchangeState.Normal;
            }
            else
            {
                return ExchangeState.Error;
            }
        }
        //
        //设置状态
        //
        public void setClothSaleState(string szID, ExchangeState eState, Dictionary<string, bool> rNoSaleID)
        {
            XmlNode rClothNode = m_rClothsetMap[szID];
            switch (eState)
            {
                case ExchangeState.Normal:
                    setNormalExchange(rClothNode, rNoSaleID);
                    break;
                case ExchangeState.Close:
                    setCloseExchange(rClothNode, rNoSaleID);
                    break;
                case ExchangeState.Sale:
                    setSaleExchange(rClothNode, rNoSaleID);
                    break;
                default:
                    break;
            }
        }
        //
        //获取信息
        //
        public void getXmlData(List<string> rIDlist ,ref List<string[]> rResult,Dictionary<string,bool> rNoSale)
        {
            for(int index= 0; index < rIDlist.Count;++index)
            {
                string szID = rIDlist[index];
                XmlNode rClothNode = m_rClothsetMap[szID];
                if (null != rClothNode )
                {
                    var rChildren = rClothNode.ChildNodes;
                    for(int nLoopCount = 0; nLoopCount < rChildren.Count;++nLoopCount)
                    {
                        if(rChildren[nLoopCount].Name == "Part")
                        {
                            string[] temp = new string[15];
                            if(rClothNode.Attributes["series_id"]!= null)
                            {
                                temp[0] = rClothNode.Attributes["series_id"].Value;
                            }
                            if (rClothNode.Attributes["set_id"] != null)
                            {
                                temp[1] = rClothNode.Attributes["set_id"].Value;
                            }
                            if (rClothNode.Attributes["on_shelf_time"] != null)
                            {
                                temp[2] = rClothNode.Attributes["on_shelf_time"].Value.ToString();
                            }
                            if (rClothNode.Attributes["complete_score"] != null)
                            {
                                temp[3] = rClothNode.Attributes["complete_score"].Value;
                            }
                            if (rClothNode.Attributes["set_name"] != null)
                            {
                                temp[4] = rClothNode.Attributes["set_name"].Value;
                            }
                            if (rChildren[nLoopCount].Attributes["male_id"] != null)
                            {
                                temp[6] = rChildren[nLoopCount].Attributes["male_id"].Value;
                            }
                            if (rChildren[nLoopCount].Attributes["female_id"] != null)
                            {
                                temp[8] = rChildren[nLoopCount].Attributes["female_id"].Value;
                            }
                            if(rChildren[nLoopCount].Attributes["score"] != null)
                            {
                                int score = int.Parse(rChildren[nLoopCount].Attributes["score"].Value);
                                temp[9] = (score * 10).ToString();
                                temp[10] = X51Math.X51round(score * 7.5).ToString();
                                if(!rNoSale.ContainsKey(temp[5]) && !rNoSale.ContainsKey(temp[7]))
                                {
                                    temp[11] = X51Math.X51round(score * 6).ToString();
                                }
                                temp[13] = score.ToString();
                            }
                            if(rChildren[nLoopCount].ChildNodes.Count != 0)
                            {
                                var partChildren = rChildren[nLoopCount].ChildNodes;
                                for(int partChildrenIndex = 0; partChildrenIndex < partChildren.Count;++ partChildrenIndex)
                                {
                                    if(partChildrenIndex == 0 && partChildren[partChildrenIndex].Name == "#comment")
                                    {
                                        getPartName(ref temp[5], ref temp[7], partChildren[partChildrenIndex].Value);
                                    }
                                    if(partChildren[partChildrenIndex].Name == "Obtain" &&
                                       partChildren[partChildrenIndex].Attributes["id"].Value != "1")
                                    {
                                        temp[12] = getPieceSource(partChildren[partChildrenIndex].Attributes["id"].Value);
                                    }
                                }
                            }
                            temp[14] = "1.00";
                            rResult.Add(temp);
                        }
                    }
                }
            }
        }
        //-------------------------private_function-------------------------------------------------------------
        //
        //获得名字
        //
        private void getPartName(ref string szMaleName , ref string szFemaleName,string szData)
        {
            int splitIndex = szData.IndexOf('/');
            if (splitIndex != -1)
            {
                szMaleName = szData.Substring(0, splitIndex);
                szFemaleName = szData.Substring(splitIndex + 1, szData.Length - splitIndex - 1);
            }
            else
            {
                szMaleName = szData;
                szFemaleName = szData;
            }
        }
        //
        //获得获取途径文字
        //
        private string getPieceSource(string szID)
        {
            if(m_rPiecesSourceExsitMap.ContainsKey(szID))
            {
                return m_rPartsSourceExsitMap[szID];
            }
            else
            {
                return string.Empty;
            }
        }
        //
        //设置兑换状态
        //
        private void setCloseExchange(XmlNode rClothNode, Dictionary<string, bool> rNoSaleID)
        {
            //去掉offer_enable字段
            if(rClothNode.Attributes["offer_enable"] != null)
            {
                rClothNode.Attributes.Remove(rClothNode.Attributes["offer_enable"]);
            }
            var rChildren = rClothNode.ChildNodes;
            for (int index = 0; index < rChildren.Count;++index)
            {
                if(rChildren[index].Name == "Part")
                {
                    //去掉part offer_cost字段
                    if(rChildren[index].Attributes["offer_cost"] != null)
                    {
                        rChildren[index].Attributes.Remove(rChildren[index].Attributes["offer_cost"]);
                    }
                    //去掉cost = “-1”
                    if(rChildren[index].Attributes["cost"] != null)
                    {
                        rChildren[index].Attributes["cost"].Value = "-1";
                    }
                }
            }
        }
        private void setNormalExchange(XmlNode rClothNode, Dictionary<string, bool> rNoSaleID)
        {
            //去掉offer_enable字段
            if (rClothNode.Attributes["offer_enable"] != null)
            {
                rClothNode.Attributes.Remove(rClothNode.Attributes["offer_enable"]);
            }
            var rChildren = rClothNode.ChildNodes;
            for (int index = 0; index < rChildren.Count; ++index)
            {
                if (rChildren[index].Name == "Part")
                {
                    //去掉cost = “-1”
                    if (rChildren[index].Attributes["cost"] != null && null != rChildren[index].Attributes["score"])
                    {
                        int nCost = X51Math.X51round( int.Parse(rChildren[index].Attributes["score"].Value) * 7.5);
                        rChildren[index].Attributes["cost"].Value = nCost.ToString();
                    }
                }
            }
        }
        private void setSaleExchange(XmlNode rClothNode, Dictionary<string, bool> rNoSaleID)
        {
            //去掉offer_enable字段
            if (rClothNode.Attributes["offer_enable"] != null)
            {
                rClothNode.Attributes["offer_enable"].Value = "1";
            }
            else
            {
                XmlAttribute rNewAttribute = getDoc().CreateAttribute("offer_enable");
                rNewAttribute.Value = "1";
                rClothNode.Attributes.Append(rNewAttribute);
            }
            var rChildren = rClothNode.ChildNodes;
            for (int index = 0; index < rChildren.Count; ++index)
            {
                if (rChildren[index].Name == "Part")
                {
                    //去掉cost = “-1”
                    if (rChildren[index].Attributes["cost"] != null && null != rChildren[index].Attributes["score"])
                    {
                        int nCost = X51Math.X51round(int.Parse(rChildren[index].Attributes["score"].Value) * 7.5);
                        int nOfferCost = X51Math.X51round(int.Parse(rChildren[index].Attributes["score"].Value) * 6);
                        rChildren[index].Attributes["cost"].Value = nCost.ToString();
                        if(null == rChildren[index].Attributes["offer_cost"])
                        {
                            XmlAttribute rNewAttribute = getDoc().CreateAttribute("offer_cost");
                            rNewAttribute.Value = nOfferCost.ToString();
                            rChildren[index].Attributes.Append(rNewAttribute); 
                        }
                        else
                        {
                            rChildren[index].Attributes["offer_cost"].Value = nOfferCost.ToString();
                        }
                    }
                }
            }
        }
        private bool isCloseExchange(XmlNode rClothNode, Dictionary<string, bool> rNoSaleID)
        {
            var rChildren = rClothNode.ChildNodes;
            for (int index = 0; index < rChildren.Count; ++index)
            {
                if(rChildren[index].Name == "Part")
                {
                    if (rNoSaleID.ContainsKey(rChildren[index].Attributes["female_id"].Value)
                        || rNoSaleID.ContainsKey(rChildren[index].Attributes["male_id"].Value))
                    {
                        continue;
                    }
                    else
                    {
                        if (rChildren[index].Attributes["cost"].Value == "-1")
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
            }
            return false;
        }
        private bool isSaleExchange(XmlNode rClothNode , Dictionary<string, bool> rNoSaleID)
        {
            var rChildren = rClothNode.ChildNodes;
            if(rClothNode.Attributes["offer_enable"] != null && rClothNode.Attributes["offer_enable"].Value == "1")
            {
                for (int index = 0; index < rChildren.Count; ++index)
                {
                    if (rChildren[index].Name == "Part")
                    {
                        if (rNoSaleID.ContainsKey(rChildren[index].Attributes["female_id"].Value)
                            || rNoSaleID.ContainsKey(rChildren[index].Attributes["male_id"].Value))
                        {
                            continue;
                        }
                        else
                        {
                            if (rChildren[index].Attributes["cost"].Value != "-1")
                            {
                                return true;
                            }
                            else
                            {
                                return false;
                            }
                        }
                    }
                }
            }
            return false;
        }
        private bool isNormalExchange(XmlNode rClothNode, Dictionary<string, bool> rNoSaleID)
        {
            var rChildren = rClothNode.ChildNodes;
            if (rClothNode.Attributes["offer_enable"] == null)
            {
                for (int index = 0; index < rChildren.Count; ++index)
                {
                    if (rChildren[index].Name == "Part")
                    {
                        if (rNoSaleID.ContainsKey(rChildren[index].Attributes["female_id"].Value)
                            || rNoSaleID.ContainsKey(rChildren[index].Attributes["male_id"].Value))
                        {
                            continue;
                        }
                        else
                        {
                            if (rChildren[index].Attributes["cost"].Value != "-1")
                            {
                                return true;
                            }
                            else
                            {
                                return false;
                            }
                        }
                    }
                }
            }
            return false;
        }
        //
        //格式化输出
        //
        private void fomatXml(ref StringBuilder rOutput)
        {
            var x = getDoc().ChildNodes;
            if (x.Count <= 0)
            {
                return;
            }
            for (int i = 0; i < x.Count; i++)
            {
                if (x[i].Name == "SeriesClothsetDynamic")
                {
                    rOutput.Append("<SeriesClothsetDynamic>\r\n");
                    fomatXmlHelper(x[i], ref rOutput, 1, true);
                    rOutput.Append("</SeriesClothsetDynamic>");
                }
                else
                {
                    rOutput.Append(x[i].OuterXml);
                    rOutput.AppendLine("");
                }
            }
        }
        private void fomatXmlHelper(XmlNode xmlNode, ref StringBuilder rOutput, int level, bool isFirstSpace)
        {
            var x = xmlNode.ChildNodes;
            if (x.Count <= 0)
            {
                return;
            }

            for (int i = 0; i < x.Count; i++)
            {
                if (x[i].HasChildNodes)
                {//has child
                    if (!(i == 0 && !isFirstSpace))
                    {
                        AppendSpace(ref rOutput, level);
                    }
                    if (specialNode(x[i]))
                    {
                        rOutput.Append(getNodePre(x[i]));
                        fomatXmlHelper(x[i], ref rOutput, level + 1, false);
                    }
                    else
                    {
                        rOutput.Append(getNodePre(x[i]) + "\r\n");
                        fomatXmlHelper(x[i], ref rOutput, level + 1, true);
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
        //
        //区别格式特别的节点
        //
        private bool specialNode(XmlNode rNodex)
        {
            return ("text" == rNodex.Name || "ExtraRewards" == rNodex.Name);
        }
        //
        //添加空格
        //
        private void AppendSpace(ref StringBuilder rBuilder, int level)
        {
            for (int j = 0; j < level; ++j)
            {
                rBuilder.Append("  ");
            }
        }
        //
        //获得 节点的开始部分
        //
        private string getNodePre(XmlNode xmlNode)
        {
            StringBuilder rBuilder = new StringBuilder();
            rBuilder.Append("<");
            rBuilder.Append(xmlNode.Name);
            if (xmlNode.Attributes != null)
            {
                for (int index = 0; index < xmlNode.Attributes.Count; ++index)
                {
                    rBuilder.Append(" " + xmlNode.Attributes[index].Name);
                    rBuilder.Append("=\"" + xmlNode.Attributes[index].Value + "\"");
                }
            }
            rBuilder.Append(">");
            return rBuilder.ToString();
        }
        //
        //获得节点的结束部分
        //
        private string getNodeEnd(XmlNode xmlNode)
        {
            StringBuilder rBuilder = new StringBuilder();
            rBuilder.Append("</");
            rBuilder.Append(xmlNode.Name);
            rBuilder.Append(">");
            return rBuilder.ToString();
        }
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
                        if(!m_rClothsetMap.ContainsKey(nSubID.ToString()))
                        {
                            m_rClothsetMap.Add(nSubID.ToString(), rNode);
                        }
                        return false;
                    }
                }
                else
                {
                    if (rNode.Name == "PieceObtainText")
                    {
                        for (int index = 0; index < rNode.ChildNodes.Count; ++index)
                        {
                            if(rNode.ChildNodes[index].Name == "text")
                            {
                                m_rPiecesSourceExsitMap.Add(rNode.ChildNodes[index + 1].Value, rNode.ChildNodes[index].Attributes["context"].Value);
                            }
                            index++;
                        }
                        m_rPieceSourceParent = rNode;
                    }
                    else if (rNode.Name == "ClothsetObtainText")
                    {
                        for (int index = 0; index < rNode.ChildNodes.Count; ++index)
                        {
                            if (rNode.ChildNodes[index].Name == "text")
                            {
                                m_rPartsSourceExsitMap.Add(rNode.ChildNodes[index].Attributes["obtain_id"].Value, rNode.ChildNodes[index].Attributes["context"].Value);
                            }
                            index++;
                        }
                        this.m_rPartSouceParent = rNode;
                    }
                    else if(rNode.Name == "ExtraRewards")
                    {
                        m_rLastRewardsNode = rNode;
                    }
                }
                return true;
            });
            foreachNode(rOperator);
        }
    
        //
        //时效获取帮助函数
        //
        private string getTimeLimitHelper(string szContent)
        {
            if(szContent == "永久")
            {
                return 0.ToString();
            }
            else
            {
                int indexDay = szContent.IndexOf("天");
                string szDays = szContent.Substring(0, indexDay);
                int indexCount = szContent.IndexOf("个");
                string szCounts = szContent.Substring(indexDay + 1, indexCount - indexDay - 1);
                return szDays + "/" + szCounts;
            }
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
