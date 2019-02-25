using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;


namespace X51Tools.Global
{
    class InternationalNamedAndIDMap
    {
        //brief
        //InternationalNamedAndIDMap,主要进行国际化命名目录下文件的加载，以及解析，
        //2019/9/21 当前支持对存在 "level_inventory_dress.csv", "level_inventory_item.csv", "level_inventory_medal.csv"
        //文件的目录进行加载并对上述文件进行解析，通过识别特定前缀，构建ID与名字的映射，以及名字与ID的映射。
        //提供get接口 输入KEY 以及性别的方式获取，提供rebuild接口用于路径的改变，一般在初始化或者重新建立映射的时候调用。
        //
        string[] m_rFileNameArr = { "level_inventory_dress.csv", "level_inventory_item.csv", "level_inventory_medal.csv" };
        string[] m_rIDPreArr = { "str_res_dress_name_", "str_res_item_name_", "str_res_medal_name_" };
        Dictionary<string, string> m_rID2NameMap;
        Dictionary<string, List<string>> m_rName2IDMap;
        private string m_szSrcPath;
        public InternationalNamedAndIDMap()
        {
            m_rID2NameMap = new Dictionary<string, string>();
            m_rName2IDMap = new Dictionary<string, List<string>>();
        }
        //
        //初始化接口
        //
        public void rebuild(string szSrcFolder, ref string szError)
        {
            if (!checkSrcTable(szSrcFolder, ref szError))
            {
                return;
            }
            m_rID2NameMap.Clear();
            m_rName2IDMap.Clear();
            if (m_szSrcPath != szSrcFolder)
            {
                m_szSrcPath = szSrcFolder;
            }
            for (int index = 0; index < this.m_rFileNameArr.Length; ++index)
            {//读取标准表文件
                string szAbsPath = szSrcFolder + @"\" + m_rFileNameArr[index];
                csvReader(szAbsPath, ref szError);
            }
            //log
            szError = "read csv over!";
        }
        //
        //根据ID获取名字
        //
        public string getName(string szID, bool isMale, ref string szError)
        {
            if (this.m_rID2NameMap.ContainsKey(szID))
            {
                string szResult = m_rID2NameMap[szID];
                if (szResult != "")
                {//查的到对应的名字
                    if (checkIsSuitSex(szID, isMale))
                    {//性别正确
                        return szResult;
                    }
                }
            }
            //error
            //不正确才会走到这
            return "";
        }
        //
        //根据名字获取ID
        //
        public string getID(string szName, bool isMale, ref string szError)
        {

            if (m_rName2IDMap.ContainsKey(szName))
            {
                List<string> rIDlist = m_rName2IDMap[szName];
                if (rIDlist.Count == 1)
                {//名字对应一个ID
                    if (checkIsSuitSex(rIDlist[0], isMale))
                    {
                        return rIDlist[0];
                    }
                }
                else
                {
                    if (rIDlist.Count < 3)
                    {//名字对应2个ID
                        string szFirstID = rIDlist[0];
                        string szSecondID = rIDlist[1];
                        if (checkIDisSameItemDifSex(szFirstID, szSecondID))
                        {//检查两个ID是否性别不同
                            if (checkIsSuitSex(szFirstID, isMale))
                            {
                                return szFirstID;
                            }
                            else if (checkIsSuitSex(szSecondID, isMale))
                            {
                                return szSecondID;
                            }
                        }
                    }
                }
                //error..
                return "";
            }
            else
            {//找不到映射
             //error.
                return "";
            }
        }

        //private
        private bool checkIDisSameItemDifSex(string ID, string szID)
        {
            char[] ID_1 = ID.ToCharArray();
            char[] ID_2 = szID.ToCharArray();
            if (ID_1[0] == ID_2[0])
            {
                if (ID_2[0] == '1' || ID_2[0] == '7' || ID_2[0] == '4')
                {
                    if ((ID_1[1] == '1' && ID_2[1] == '0') || (ID_2[1] == '1' && ID_1[1] == '0'))
                        return true;
                }
            }
            return false;
        }
        //
        // 检查性别
        //
        private bool checkIsSuitSex(string szID, bool isMale)
        {
            char[] ID_arr = szID.ToCharArray();
            if (ID_arr[0] == '1' || ID_arr[0] == '7' || ID_arr[0] == '4')
            {
                if (isMale)
                {
                    return '0' == ID_arr[1];
                }
                else
                {
                    return '1' == ID_arr[1];
                }
            }
            else
            {
                return true;
            }
        }
        //
        //源文件读取方法
        //
        private  void csvReader(string szPath,  ref string rInfoOutput)
        {
            string szExcelFilePath = szPath.Trim();
            FileStream rFile = new FileStream(szExcelFilePath, FileMode.Open, FileAccess.Read);
            StreamReader rReader = new StreamReader(rFile, Encoding.Default);
            try
            {//每个工作表都查 索引从1开始
                string strReadline;
                while (!rReader.EndOfStream)
                {
                    strReadline = rReader.ReadLine();
                    string[] szStrArr = strReadline.Split(',');
                    if (2 == szStrArr.Length)
                    {
                        string szIDwithPre = szStrArr[0];
                        string szName = szStrArr[1];
                        //to do
                        createPair(szIDwithPre, szName);
                    }
                    else
                    {
                        //error.
                    }
                }

            }
            catch (Exception ex)
            {
                //error.
            }
            finally
            {
                rReader.Close();
                rFile.Close();
            }
            return;
        }
        //
        // function SaveFuncDef:
        //当标准表数据中存在重复的ID的时候 返回false 以为源数据有问题（暂定）
        //
        private bool createPair(string szIDWithPre, string szName)
        {
            string szResultID = "";
            //不符合前缀
            if (!resolvePreInRule(szIDWithPre, ref szResultID))
            {
                return true;
            }
            //这个函数极其之慢 慢的不行
            //if(string.IsNullOrWhiteSpace(szResultID))
            //{
            //      return true;
            //}
            //创建映射
            //ID2NAMEMAP
            if (this.m_rID2NameMap.ContainsKey(szResultID))
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
        //
        //检查输入文本前缀是否符合规则
        //
        private bool resolvePreInRule(string szIDWithPre, ref string szResultID)
        {
            for (int index = 0; index < m_rIDPreArr.Length; ++index)
            {
                if (szIDWithPre.StartsWith(m_rIDPreArr[index]))
                {
                    szResultID = szIDWithPre.Substring(m_rIDPreArr[index].Length, szIDWithPre.Length - m_rIDPreArr[index].Length);
                    return true;
                }
            }
            return false;
        }

        //
        //检查文件源文件是否存在
        //
        private bool checkSrcTable(string szSrcFolder, ref string rInfoOutPut)
        {
            bool rResult = true;
            for (int index = 0; index < m_rFileNameArr.Length; ++index)
            {
                if (!File.Exists(szSrcFolder + @"\" + m_rFileNameArr[index]))
                {
                    rResult = false;
                    rInfoOutPut = "缺少" + @"\" + m_rFileNameArr[index] + "文件" + '\n';
                }
            }
            return rResult;
        }
    }
}
