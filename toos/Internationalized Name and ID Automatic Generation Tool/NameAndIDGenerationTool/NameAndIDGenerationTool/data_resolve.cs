using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace NameAndIDGenerationTool
{
    class DataResolve
    {
        string[] m_rFileNameArr = { "level_inventory_dress.csv", "level_inventory_item.csv", "level_inventory_medal.csv" };
        string[] m_rIDPreArr = { "str_res_dress_name_", "str_res_item_name_", "str_res_medal_name_" };
        Dictionary<string, string> m_rID2NameMap;
        Dictionary<string, List<string>> m_rName2IDMap;

        public DataResolve()
        {
            m_rID2NameMap = new Dictionary<string, string>();
            m_rName2IDMap = new Dictionary<string, List<string>>();
        }

        public bool checkSrcTable(string szSrcFolder, ref System.Windows.Forms.RichTextBox rInfoOutPut)
        {
            bool rResult = true;
            for (int index = 0; index < m_rFileNameArr.Length; ++index)
            {
                if (!ExcelOperator.check_src_file_exist(szSrcFolder + @"\" + m_rFileNameArr[index]))
                {
                    rResult = false;
                    rInfoOutPut.SelectionColor = Color.Red;
                    rInfoOutPut.AppendText("缺少"+ @"\" + m_rFileNameArr[index] +"文件"+ '\n');
                }
            }
            return rResult;
        }
        //初始化
        public void initialize(string szSrcFolder, ref System.Windows.Forms.RichTextBox rInfoOutPut)
        {
            if(!checkSrcTable(szSrcFolder,ref rInfoOutPut))
            {
                return;
            }
            var rSaveFunc = new SaveData(SaveFuncDef);
            for (int index = 0; index < this.m_rFileNameArr.Length; ++index)
            {//读取标准表文件
                string szAbsPath = szSrcFolder + @"\" + m_rFileNameArr[index];
                ExcelOperator.read_file_N_append_data(szAbsPath, rSaveFunc, ref rInfoOutPut);
            }
        }
        // function SaveFuncDef:
        //当标准表数据中存在重复的ID的时候 返回false 以为源数据有问题（暂定）
        private bool SaveFuncDef(string szIDWithPre ,string szName)
        {
            string szResultID = "";
            //不符合前缀
            if (!resolvePreInRule(szIDWithPre,ref szResultID))
            {
                return true;
            }
            if(string.IsNullOrWhiteSpace(szResultID))
            {
                return true;
            }
             //创建映射
             //ID2NAMEMAP
            if(this.m_rID2NameMap.ContainsKey(szResultID))
            {
                return false;
            }
            else
            {
                m_rID2NameMap.Add(szResultID, szName);
            }
            //NAME2IDMAP
            //讲道理上边验过重这边不应该有重复的ID
            if (this.m_rName2IDMap.ContainsKey(szName))
            {
                this.m_rName2IDMap[szName].Add(szResultID);
            }
            else
            {
                List<string> rIDlist = new List<string>();
                rIDlist.Add(szResultID);
                this.m_rName2IDMap.Add(szName, rIDlist);
            }
            return true; 
        }

        private bool resolvePreInRule(string szIDWithPre,ref string szResultID)
        {
            for (int index = 0; index < m_rIDPreArr.Length; ++index)
            {
                if(szIDWithPre.StartsWith(m_rIDPreArr[index]))
                {
                    szResultID = szIDWithPre.Substring(m_rIDPreArr[index].Length , szIDWithPre.Length - m_rIDPreArr[index].Length);
                    return true;
                }
            }
            return false;
        }

        public void testFunc(ref System.Windows.Forms.RichTextBox rOutput)
        {
            rOutput.SelectionColor = Color.Black;
            ExcelOperator.write_temp_file(ref this.m_rID2NameMap);
//             rOutput.AppendText(" ID:");
//             foreach (var temp in this.m_rID2NameMap)
//             {
//                 rOutput.AppendText(temp.Key);
//                 rOutput.AppendText("  ,  ");
//                 rOutput.AppendText(temp.Value + "\n");
//             }
        }
    }
}
