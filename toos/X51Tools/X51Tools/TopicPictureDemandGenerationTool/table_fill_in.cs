using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using X51Tools.Global;
using System.IO;


namespace X51Tools.TopicPictureDemandGenerationTool
{
    public delegate string getSaveAsPath();
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

        private TableCoverStrInput m_rCoverInput;
        public TableFillIn()
        {
            m_rNameAndIDMap = new InternationalNamedAndIDMap();
            m_rExcelOperator = new TopicExcelOperator();
            m_rXmlOperator = new TopicConfigXmlOperator();
            m_rNoSaleList = new Dictionary<string, int>();
            m_rCoverInput = new TableCoverStrInput();
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

        public void execute(ref System.Windows.Forms.RichTextBox rLog, getSaveAsPath rSavePath)
        {
            string szError = "";
            rLog.Clear();
            //错误提示 null m_szConfigPath m_szInputPath
            m_rXmlOperator.build(m_szConfigPath);
            initNoSale();
            m_rExcelOperator.excelFillIn(m_szInputPath, TableRowWriteFunc, ref szError);
            string szXmlSavePath = rSavePath();
            if (string.Empty != szXmlSavePath)
            {
                generateXmlFile(szXmlSavePath);
            }
            rLog.AppendText(szError);
            writeLopFile(szError);
            m_rExcelOperator.saveAsNewFile();
        }
        //
        //书写dump
        //
        private void writeLopFile(string szString)
        {
            FileStream rFile = new FileStream("error.txt", FileMode.OpenOrCreate, FileAccess.Write);
            StreamWriter rWriter = new StreamWriter(rFile);
            rWriter.Write(szString);
            rWriter.Close();
            rFile.Close();
        }
        //
        //生成新的配置文件
        //
        public void generateXmlFile(string rSavePath)
        {
            errorHandle rErrorFunc = new errorHandle((string szError)=> 
            {
                //错误处理
            });
            //获取途径
            int nSourceValidDataCount = this.m_rExcelOperator.getValidDataRowCount("系列套装部件产出途径文本预设及系列碎片产出文本");
            if(-1 != nSourceValidDataCount)
            {
                for (int index = 0; index < nSourceValidDataCount; ++index)
                {
                    string[] rRowData = new string[4];
                   if( this.m_rExcelOperator.getExcelRow(ref rRowData,2 + index , "系列套装部件产出途径文本预设及系列碎片产出文本"))
                    {
                        this.m_rXmlOperator.appendSource(rRowData, rErrorFunc);
                    }
                }
            }
            //额外奖励
            int nRewardsValidDataCount = this.m_rExcelOperator.getValidDataRowCount("额外奖励配置");
            if (-1 != nRewardsValidDataCount)
            {
                for (int index = 0; index < nRewardsValidDataCount; ++index)
                {
                    string[] rRowData = new string[6];
                    if (this.m_rExcelOperator.getExcelRow(ref rRowData, 2 + index, "额外奖励配置"))
                    {
                        this.m_rXmlOperator.appendExtraReward(rRowData);
                    }
                }
            }
            //套装
            int nClothsValidDataCount = this.m_rExcelOperator.getValidDataRowCount("系列套装配置");
            if (-1 != nClothsValidDataCount)
            {
                string szSetName = "";
                int nSourceRowNumber = 2;
                string szCoverString = "";
                for (int index = 0; index < nClothsValidDataCount; ++index)
                {
                    string[] rRowData = new string[15];
                    if (this.m_rExcelOperator.getExcelRow(ref rRowData, 2 + index, "系列套装配置"))
                    {
                        if(szSetName != rRowData[4])
                        {
                            if(szSetName != "")
                            {
                                ++nSourceRowNumber;
                            }
                            szSetName = rRowData[4];
                            m_rCoverInput.initialize(rRowData[4]);
                            m_rCoverInput.ShowDialog();
                            if(m_rCoverInput.getCommit())
                            {
                                szCoverString = m_rCoverInput.getCoverInputStr();
                            }
                        }
                        this.m_rXmlOperator.appendClothset(rRowData, nSourceRowNumber, szCoverString);
                    }
                }
            }
            this.m_rXmlOperator.outputNewXml(rSavePath);
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
            string szFragmentSource = "碎片获取途径" + (this.m_rXmlOperator.getPieceObtainID() + 1).ToString();
            string szPartSource = (this.m_rXmlOperator.getClothsetObtainID() + 1 ).ToString();

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
