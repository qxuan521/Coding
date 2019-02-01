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
        SrcPath,
        ResultPath
    };
    static class PersistenceFileStream
    {
        private static string szPersiztenceFilePath =  "PersistenceData.data";
        public static void dataWrite(DataInfileType[] rTypeArr, string[] szData)
        {
            FileStream rFile = new FileStream(szPersiztenceFilePath, FileMode.OpenOrCreate, FileAccess.Write);
            for (int index = 0; index < szData.Length;++index)
            {
                if(szData[index] == null)
                {
                    continue;
                }
                byte[] rTypebuffer = BitConverter.GetBytes((int)rTypeArr[index]);
                byte[] rBuffer = System.Text.Encoding.UTF8.GetBytes(szData[index]);
                byte[] rSizeBuffer = BitConverter.GetBytes(rBuffer.Length);
                rFile.Write(rTypebuffer, 0, 4);
                rFile.Write(rSizeBuffer, 0, 4);
                rFile.Write(rBuffer, 0, rBuffer.Length);
            }
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
        public static string[] dataRead()
        {
            string[] rResultArr = new string[2];
            if (!NameAndIDGenerationTool.ExcelOperator.checkSrcExist(szPersiztenceFilePath))
            {
                return rResultArr;
            }
            FileStream rFile = new FileStream(szPersiztenceFilePath, FileMode.Open, FileAccess.Read);
            try
            {
                //标准文件路径
                int index = 0;
                while(true)
                {
                    byte[] rBuffer = new byte[4];
                    rFile.Read(rBuffer, 0, 4);
                    DataInfileType nType = (DataInfileType)BitConverter.ToInt32(rBuffer, 0);
                    rFile.Read(rBuffer, 0, 4);
                    int nSize = BitConverter.ToInt32(rBuffer, 0);
                    rBuffer = new byte[nSize];
                    rFile.Read(rBuffer, 0, nSize);
                    string szResult = Encoding.UTF8.GetString(rBuffer);
                    if(nType == DataInfileType.SrcPath)
                    {
                        rResultArr[0] = szResult;
                    }
                    else if(nType == DataInfileType.ResultPath)
                    {
                        rResultArr[1] = szResult;

                    }
                    else
                    {
                        rFile.Close();
                        return rResultArr;
                    }
                }
            }
            catch(Exception ex)
            {
                rFile.Close();
                return rResultArr;
            }
        }
    }
  
}
