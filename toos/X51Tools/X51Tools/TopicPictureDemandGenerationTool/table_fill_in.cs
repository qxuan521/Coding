using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using X51Tools.Global;

namespace X51Tools.TopicPictureDemandGenerationTool
{
    public partial class TableFillIn
    {
        //brief
        //完整版输入表填写以及配置文件生成功能类
        //

        private Global.InternationalNamedAndIDMap   m_rNameAndIDMap;
        private TopicExcelOperator                  m_rExcelOperator;
        private TopicConfigXmlOperator              m_rXmlOperator;
        private string m_szInputPath;
        private string m_szConfigPath;
        private string m_szNoSalePath;
        //
        //单次功能全局保持
        //
        private string m_szTempLastSubName = "";
        private Dictionary<string,int> m_rNoSaleList;
        private int m_nTempLastSubID = -1;
        public TableFillIn()
        {
            m_rNameAndIDMap = new InternationalNamedAndIDMap();
            m_rExcelOperator = new TopicExcelOperator();
            m_rXmlOperator = new TopicConfigXmlOperator();
            m_rNoSaleList = new Dictionary<string, int>();
        }

        public void setInternationalPath(string szPath, ref string szError)
        {
            m_rNameAndIDMap.rebuild(szPath, ref szError);
        }

        public void setInputExcelPath(string szPath)
        {
            m_szInputPath = szPath;
        }

        public void setConfigFilePath(string szPath)
        {
            m_szConfigPath = szPath;
        }

        public void setNoSalePath(string szPath)
        {
            m_szNoSalePath = szPath;
        }

        public void execute(ref System.Windows.Forms.RichTextBox rLog)
        {
            string szError = "";
            rLog.Clear();
            //错误提示 null m_szConfigPath m_szInputPath
            m_rXmlOperator.build(m_szConfigPath);
            initNoSale();
            FillInFinishFunc rFinishFunc = new FillInFinishFunc(() => 
            {

            });
            m_rExcelOperator.excelFillIn(m_szInputPath, TableRowWriteFunc, rFinishFunc, ref szError);
            rLog.AppendText(szError);

        }
        public bool TableRowWriteFunc(ref string[] rRowData)
        {
            switch(rRowData.Length)
            {
                case 15:
                    return suitConfig(ref rRowData);
                case 4:
                    return fragmentSource(ref rRowData);
                default:
                    return false;
            }
        }

        //
        //
        //
        private bool suitConfig(ref string[] rRowData)
        {
            string szError = "";
            //子系列ID(xml)
            string szMatherID = rRowData[0];
            string szSubName = rRowData[4];
            if (szSubName == m_szTempLastSubName)
            {//不去做无效验证 1.首先m_szTempLastSubName初始为 "" 表中内容不可能会和他相同（除非表有问题）
                rRowData[1] = m_nTempLastSubID.ToString();
            }
            else
            {
                m_nTempLastSubID = this.m_rXmlOperator.getSubNextID(szMatherID) + 1;
                rRowData[1] = m_nTempLastSubID.ToString();
                m_szTempLastSubName = szSubName;
            }
            //子系列ID(xml) - end
            //道具名
            string szIDMale = rRowData[5];
            string szIDFemale = rRowData[7];
            string szNameMale = this.m_rNameAndIDMap.getName(szIDMale, true, ref szError);
            string szNameFemale = this.m_rNameAndIDMap.getName(szIDFemale, false, ref szError);
            if(szNameFemale != "")
            {
                rRowData[8] = szNameFemale;
            }
            if(szNameMale != "")
            {
                rRowData[6] = szNameMale;
            }
            //道具名 - end 
            //计算兑换所需碎片和首次兑换优惠碎片书以及套部件见分数
            int nPrice = int.Parse(rRowData[9]);
            int nFragmentCount = X51Math.X51round(nPrice * 0.75);
            int nSaleFragmentCount = X51Math.X51round(nPrice * 0.6);
            int nPartScore = X51Math.X51round(nPrice * 0.1);
            rRowData[10] = nFragmentCount.ToString();
            if(isNoSale(szIDMale, szIDFemale))
            {
                rRowData[11] = "";
            }
            else
            {
                rRowData[11] = nSaleFragmentCount.ToString();
            }
            rRowData[13] = nPartScore.ToString();
            //计算兑换所需碎片和首次兑换优惠碎片书以及套部件见分数 - end
            return true;
        }
        //
        //
        //
        private bool fragmentSource(ref string[] rRowData)
        {
            //ID(xml)
            string szFragmentSource = "碎片获取途径" + this.m_rXmlOperator.getPieceObtainID().ToString();
            string szPartSource = this.m_rXmlOperator.getClothsetObtainID().ToString();

            rRowData[0] = szFragmentSource;
            rRowData[2] = szPartSource;
            //
            return false;

        }
        //
        //
        //
        private bool extraReward(ref string[] rRowData)
        {

            return false;
        }
        //
        //
        //
        private void clearTemp()
        {
            this.m_szTempLastSubName = "";
            this.m_nTempLastSubID = -1;
        }

        //
        //
        //
        private void initNoSale()
        {
            m_rNoSaleList.Clear();
            TopicExcelOperator rExl = new TopicExcelOperator();
            rExl.readNoSaleTable(m_szNoSalePath, ref m_rNoSaleList);
        }
        //
        //
        //
        private bool isNoSale(string szMaleID,string szFemaleID)
        {
            return m_rNoSaleList.ContainsKey(szMaleID) || m_rNoSaleList.ContainsKey(szFemaleID);
        }
    }
}
