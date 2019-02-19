using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Global
{
    static class Tools
    {
        /*
         * 获得全路径中的名字（包含后缀名）
         */
        public static string getNameFromFullPath(string szFullPath)
        {
            int index = szFullPath.LastIndexOf("\\");
            if (-1 == index)
            {
                index = szFullPath.LastIndexOf("/");
                if (-1 == index)
                {
                    return "";
                }
                else
                {
                    string szResult = szFullPath.Substring(index);
                    return szResult;
                }
            }
            else
            {
                string szResult = szFullPath.Substring(index);
                return szResult;
            }
        }
    }
}
