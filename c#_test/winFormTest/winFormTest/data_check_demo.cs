using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace winFormTest
{
    class data_check_demo
    {
        private Dictionary<string, string>      m_rID2NameMap;
        private Dictionary<string, string[]>    m_rNameMap;
        private excel_operator m_excel_Operator = new excel_operator();
        private string[] m_rSrcfileArr = 
        {
            "hahaha",
            "hehehe",
            "heiheihei"
        };
        private string[] m_rSrcPrefixArr =
        {
            "AA_",
            "bb_",
            "ccc_"
        };
        data_check_demo()
        {

        }
        ~data_check_demo()
        {

        }
        bool load_src_table_file(string szFolderPath)
        {
            for (int index = 0; index < m_rSrcfileArr.Length; ++index)
            {
               if( ! excel_operator.check_src_file_exist(szFolderPath + @"\"+ m_rSrcfileArr[index]))
                {
                    //xxx is not exist!!;
                }
            }
            return true;
        }
    }
}
