using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace X51Tools.TopicPictureDemandGenerationTool
{
    class GetHistoryData
    {
        //property
        string m_szConfigPath = string.Empty;
        string m_szSearchList = string.Empty;

        TopicConfigXmlOperator  m_rXmlOperator;
        TopicExcelOperator m_rExcelOperator;

        string[] m_rHeader = { "母系列" ,"子系列" ,"上架时间" , "手机完成额外分数基数(z)","系列名称","男ID","男物品","女ID","女物品","市场参考价","兑换所需系列碎片","首次兑换优惠所需碎片","产出途径","系列套装部件分数（y）","得分系数（x）"};
        public GetHistoryData()
        {
            m_rXmlOperator = new TopicConfigXmlOperator();
            m_rExcelOperator = new TopicExcelOperator();
        }

        //
        //路径设置接口
        //
        public void setConfigPath(string szPath)
        {
            m_szConfigPath = szPath;
        }
        public void setSearchList(string szPath)
        {
            m_szSearchList = szPath;
        }

        public void excute()
        {
            this.m_rXmlOperator.build(m_szConfigPath);
            List<string> rIDList = new List<string>();
            Dictionary<string, bool> rNoSale = new Dictionary<string, bool>();
            m_rExcelOperator.readHistoryIDList(m_szSearchList, ref rIDList,ref rNoSale);
            List<string[]> rResult = new List<string[]>();
            m_rXmlOperator.getXmlData(rIDList,ref rResult, rNoSale);
            m_rExcelOperator.generateHistoryInfoTable(rResult, m_rHeader);
        }
    }
}
