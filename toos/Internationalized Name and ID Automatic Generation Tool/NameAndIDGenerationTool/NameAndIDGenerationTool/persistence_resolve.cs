using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace PersistenceResolve
{
    enum DataInfileType
    {
        Path
    };
    static class PersistenceFileStream
    {
        private static string szPersiztenceFilePath =  "PersistenceData.data";
        public static void dataWrite(DataInfileType eType,int nSize,string szData)
        {
            FileStream rFile = new FileStream(szPersiztenceFilePath, FileMode.OpenOrCreate, FileAccess.Write);
            byte[] rBuffer = BitConverter.GetBytes((int)eType);
            rFile.Write(rBuffer,0,4);
            rBuffer = System.Text.Encoding.UTF8.GetBytes(szData);

            byte[] rSizeBuffer = BitConverter.GetBytes(rBuffer.Length);
            rFile.Write(rSizeBuffer, 0, 4);

            rFile.Write(rBuffer,0, rBuffer.Length);
            rFile.Close();
            //             FileStream rFile = new FileStream(szPersiztenceFilePath, FileMode.Append, FileAccess.Write);
            //             StreamWriter rWriter = new StreamWriter(rFile, Encoding.UTF8);
            //             int nFileCount = 0;
            // 
            //             if (0 == rFile.Seek(0,SeekOrigin.Begin))
            //             {
            //                 nFileCount = 0;
            //             }
            //             else
            //             {
            //                 byte[] rBuffer = new byte[4];
            //                 rFile.Read(rBuffer, 0, 4);
            //                 nFileCount = BitConverter.ToInt32(rBuffer,0);
            //             }
            //             {//填写有效数据个数
            //                 ++nFileCount;
            //                 byte[] rBuffer = BitConverter.GetBytes(nFileCount);
            //                 rFile.Write(rBuffer,0,4);
            //             }
        }
        public static string dataRead(DataInfileType eType)
        {
            if (!NameAndIDGenerationTool.ExcelOperator.checkSrcExist(szPersiztenceFilePath))
            {
                return "";
            }
            FileStream rFile = new FileStream(szPersiztenceFilePath, FileMode.Open, FileAccess.Read);
            try
            {
                byte[] rBuffer = new byte[4];
                rFile.Read(rBuffer, 0, 4);
                DataInfileType nType = (DataInfileType)BitConverter.ToInt32(rBuffer, 0);
                rFile.Read(rBuffer, 0, 4);
                int nSize = BitConverter.ToInt32(rBuffer, 0);
                rBuffer = new byte[nSize];
                string szResult = BitConverter.ToString(rBuffer);
                rFile.Close();
                return szResult;
            }
            catch(Exception ex)
            {
                rFile.Close();
                return "";
            }
        }
    }
  
}
