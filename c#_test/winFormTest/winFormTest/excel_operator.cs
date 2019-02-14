using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
namespace winFormTest
{
    class excel_operator
    {
        public static bool check_src_file_exist(string szFileName)
        {
            return File.Exists(@szFileName);
        }
    }
}
