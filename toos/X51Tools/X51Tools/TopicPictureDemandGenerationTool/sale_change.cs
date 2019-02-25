using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace X51Tools.TopicPictureDemandGenerationTool
{
    public delegate void OutputStateList(Dictionary<string, ExchangeState> rResult);
    class SaleChange
    {
        TopicExcelOperator m_rExcelOperator = new TopicExcelOperator();
        TopicConfigXmlOperator m_rXmlOperator = new TopicConfigXmlOperator();

        Dictionary<string, bool> m_rSearchMap = new Dictionary<string,bool>();
        Dictionary<string, bool> m_rNoSaleMap = new Dictionary<string, bool>();
        string m_szSearchList = string.Empty;
        string m_szConfigPath = string.Empty;
        public SaleChange()
        {

        }
        //
        //路径设置接口
        //
        public void setSearchlistPath(string szPath)
        {
            this.m_szSearchList = szPath;
        }
        public void setConfigPath(string szPath)
        {
            m_szConfigPath = szPath;
        }
        //
        //搜索接口
        //
        public void search(OutputStateList rOutputer)
        {
            m_rSearchMap.Clear();
            m_rNoSaleMap.Clear();
            m_rExcelOperator.readSearchListTable(m_szSearchList, ref this.m_rSearchMap, ref this.m_rNoSaleMap);
            m_rXmlOperator.build(m_szConfigPath);
            Dictionary<string, ExchangeState> rResult = new Dictionary<string, ExchangeState>();
            foreach (var it in this.m_rSearchMap)
            {
               ExchangeState eState = m_rXmlOperator.getClothSaleState(it.Key, this.m_rNoSaleMap);
               if(ExchangeState.Error != eState)
               {
                    rResult.Add(it.Key, eState);
               }
            }
            rOutputer(rResult);
        }

        //
        //修改接口
        //

        public void alterData(Dictionary<string,ExchangeState> rDataMap, getSaveAsPath rSavePath)
        {
            foreach (var it in rDataMap)
            {
                m_rXmlOperator.setClothSaleState(it.Key, it.Value, m_rNoSaleMap);
            }
            m_rXmlOperator.outputNewXml(rSavePath());
        }
    }
}
