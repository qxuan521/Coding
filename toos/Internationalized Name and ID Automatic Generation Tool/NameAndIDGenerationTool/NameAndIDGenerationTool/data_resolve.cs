﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using PersistenceResolve;

namespace NameAndIDGenerationTool
{
    class DataResolve
    {
        string[] m_rFileNameArr = { "level_inventory_dress.csv", "level_inventory_item.csv", "level_inventory_medal.csv" };
        string[] m_rIDPreArr = { "str_res_dress_name_", "str_res_item_name_", "str_res_medal_name_" };
        Dictionary<string, string> m_rID2NameMap;
        Dictionary<string, List<string>> m_rName2IDMap;
        private string m_szSrcPath;

        public DataResolve()
        {
            m_rID2NameMap = new Dictionary<string, string>();
            m_rName2IDMap = new Dictionary<string, List<string>>();
            m_szSrcPath = PersistenceFileStream.dataRead(DataInfileType.Path);
        }

        public string getSrcPath()
        {
            return m_szSrcPath;
        }

        public bool checkSrcTable(string szSrcFolder, ref System.Windows.Forms.RichTextBox rInfoOutPut)
        {
            bool rResult = true;
            for (int index = 0; index < m_rFileNameArr.Length; ++index)
            {
                if (!ExcelOperator.checkSrcExist(szSrcFolder + @"\" + m_rFileNameArr[index]))
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
            if (m_szSrcPath != szSrcFolder)
            {
                m_szSrcPath = szSrcFolder;
                PersistenceFileStream.dataWrite(DataInfileType.Path, m_szSrcPath.Length, m_szSrcPath);
            }
            var rSaveFunc = new SaveData(SaveFuncDef);
            for (int index = 0; index < this.m_rFileNameArr.Length; ++index)
            {//读取标准表文件
                string szAbsPath = szSrcFolder + @"\" + m_rFileNameArr[index];
                ExcelOperator.csvReader(szAbsPath, rSaveFunc, ref rInfoOutPut);
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
            //这个函数极其之慢 慢的不行
//             if(string.IsNullOrWhiteSpace(szResultID))
//             {
//                 return true;
//             }
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

        public void completionData(string szWorkFileFullPath, string szResultFileFolder, ref System.Windows.Forms.RichTextBox rInfoOutPut)
        {
            //打开第一个文件，
            OperatorFunc rOperator = new OperatorFunc(writeData);
        }
        private bool writeData(ref string szName,ref string szID,bool isMale)
        {//不允许 名字ID同时为空 不允许 名字ID同时不为空的传进来
            if(szName == "")
            {
                return findSuitableName(ref szName, ref szID, isMale);
            }
            else
            {
                return findSuitableID(ref szName, ref szID, isMale);
            }
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
        private bool findSuitableID(ref string szName, ref string szID,bool isMale)
        {
            if(m_rName2IDMap.ContainsKey(szName))
            {
                List<string> rIDlist = m_rName2IDMap[szName];

                return true;
            }
            else
            {//找不到映射
                return false;
            }
        }

        private bool findSuitableName(ref string szName, ref string szID, bool isMale)
        {

        }
    }
}