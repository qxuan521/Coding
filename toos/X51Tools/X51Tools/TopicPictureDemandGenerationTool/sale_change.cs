using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace X51Tools.TopicPictureDemandGenerationTool
{
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
        //执行接口
        //
        public void execute()
        {
            m_rExcelOperator.readSearchListTable(m_szSearchList, ref this.m_rSearchMap, ref this.m_rNoSaleMap);
            m_rXmlOperator.build(m_szConfigPath);

        }


    }
}
