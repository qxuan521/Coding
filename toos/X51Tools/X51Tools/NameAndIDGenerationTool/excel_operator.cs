using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;
using System.Drawing;
using Microsoft.Office.Interop.Excel;

namespace X51Tools.NameAndIDGenerationTool
{//ref int nManCol,ref  int nManID,ref  int nWomanCol,ref int nWomanID
    public delegate bool SaveData(string szID,string szName);
    public delegate bool OperatorFunc(ref string szID,ref string szName,bool bIsMale);
    public delegate bool OperatorCheckFunc(ref string szID,ref string szName,bool bIsMale,string address,ref System.Windows.Forms.RichTextBox rInfoOutput );
    enum OperatorHead
    {
        MaleItem = 0,
        MaleID = 1,
        FemaleItem = 2,
        FemaleID = 3,
        HeadMax
    };
    class ExcelOperator
    {
        // 0：男物品，1：男ID，2：女物品，3：女ID
        private static string[] rHeadArr = { "男物品", "男ID", "女物品", "女ID" };
        public static void setTableHeadArr(string[] headarr)
        {
            rHeadArr = headarr;
        }
        
        public static bool checkSrcExist(string szFileName)
        {
            return File.Exists(@szFileName);
        }
        //read csv table
        public static void csvReader(string szPath, SaveData rSaveFunc, ref System.Windows.Forms.RichTextBox rInfoOutput)
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
                        rSaveFunc(szIDwithPre, szName);
                    }
                    else
                    {
                    }
                }
             
            }
            catch (Exception ex)
            {
                rInfoOutput.SelectionColor = Color.Red;
                rInfoOutput.AppendText(ex.ToString());
            }
            finally
            { 
                rReader.Close();
                rFile.Close();
            }
            return ;
        }
        public static bool comReaderNSaveData(string szPath, SaveData rSaveFunc,ref System.Windows.Forms.RichTextBox rInfoOutput)
        {
            string szExcelFilePath = szPath.Trim();
            Microsoft.Office.Interop.Excel.Application excel = new Microsoft.Office.Interop.Excel.Application();
            Microsoft.Office.Interop.Excel.Workbooks wb = excel.Workbooks;
            excel.Visible = false;//设置调用引用的 Excel文件是否可见
            excel.Application.DisplayAlerts = false;
            //wb = excel.Workbooks.Open(ExcelFilePath);
            Workbook rWbk = wb.Add(szExcelFilePath);
            Sheets rWorkSheets = rWbk.Worksheets;
            try
            {//每个工作表都查 索引从1开始
                for (int index = 1; index <= rWorkSheets.Count; ++index)
                {
                    Microsoft.Office.Interop.Excel.Worksheet ws = (Microsoft.Office.Interop.Excel.Worksheet)rWorkSheets[index];
                    int rowCount = 0;//有效行，索引从1开始
                    rowCount = ws.UsedRange.Rows.Count;//赋值有效行
                    string ordernum = string.Empty;
                    string count = string.Empty;
                    //循环行
                    for (int i = 1; i <= rowCount; i++)//
                    {//将行中数据交给 代理处理
                        string szIDwithPre = ws.Cells[i, 1].Value.ToString();
                        string szName = ws.Cells[i, 2].Value.ToString();
                        rSaveFunc(szIDwithPre, szName);
                    }
                }
            }
            catch (Exception ex)
            {
                rInfoOutput.SelectionColor = Color.Red;
                rInfoOutput.AppendText(ex.ToString());
            }
            finally
            {
                /*ExcelClose(szPath, excel, rWbk);*/
            }
            return true;
        }
        public static void excelReadNCheck(string szPath, OperatorCheckFunc checkFunc, ref System.Windows.Forms.RichTextBox rInfoOutput)
        {
            string szExcelFilePath = szPath.Trim();
            Microsoft.Office.Interop.Excel.Application excel = new Microsoft.Office.Interop.Excel.Application();
            Workbooks wb = excel.Workbooks;
            excel.Visible = false;//设置调用引用的 Excel文件是否可见
            excel.Application.DisplayAlerts = false;
            //wb = excel.Workbooks.Open(ExcelFilePath);
            Workbook rWbk = wb.Add(szExcelFilePath);
            Sheets rWorkSheets = rWbk.Worksheets;
            try
            {//每个工作表都查 索引从1开始
                int[] rColNum = new int[(int)OperatorHead.HeadMax];
                bool isResultSucced = true;
                for (int index = 1; index <= rWorkSheets.Count; ++index)
                {
                    Worksheet ws = (Worksheet)rWorkSheets[index];
                    int rowCount = 0;//有效行，索引从1开始
                    rowCount = ws.UsedRange.Rows.Count;//赋值有效行
                    bool bIsHead = false;
                    bool bIsEnd = true;
                    for (int i = 1; i <= rowCount; i++)//
                    {//将行中数据交给 代理处理
                        string[] rUseFulContent = new string[(int)OperatorHead.HeadMax];
                        int nColCount = ws.UsedRange.Columns.Count;
                        for (int nLoopCount = ws.UsedRange.Column; nLoopCount <= nColCount; ++nLoopCount)
                        {//循环一行中的每一列
                            if (ws.Cells[i, nLoopCount].Value == null)
                            {
                                if (nLoopCount == 1)
                                {
                                    bIsEnd = true;
                                }
                                else
                                {
                                    bIsEnd = bIsEnd && true;
                                }
                                continue;
                            }
                            else
                            {
                                if (nLoopCount == 1)
                                {
                                    bIsEnd = false;
                                }
                                else
                                {
                                    bIsEnd = bIsEnd && false;
                                }
                            }
                            string szContent = ws.Cells[i, nLoopCount].Value.ToString().Trim();
                            bIsEnd = bIsEnd && szContent == "";
                            if (szContent == "")
                            {
                                continue;
                            }
                            if (checkIsHead(szContent))
                            {//代表当前行中存在表头
                                if (!bIsHead)
                                {
                                    bIsHead = true;
                                    clearHeadindexArr(ref rColNum);
                                }
                                //记录行列标记
                                setHeadindex(szContent, nLoopCount, ref rColNum);
                            }
                            string szAddress = ws.Cells[i, nLoopCount].Address;
                        }
                        if (bIsEnd)
                        {
                            clearHeadindexArr(ref rColNum);
                            continue;
                        }
                        if (!bIsHead && headValidaion(rColNum[0], rColNum[1], rColNum[2], rColNum[3]))
                        {
                            for (int nUsefulIndex = 0; nUsefulIndex < rColNum.Length; nUsefulIndex += 2)
                            {
                                int nColName = rColNum[nUsefulIndex];
                                int nColID = rColNum[nUsefulIndex + 1];
                                if (0 != nColID && 0 != nColName)
                                {
                                    string szName;
                                    string szID;
                                    if (ws.Cells[i, nColName].Value == null)
                                    {
                                        szName = "";
                                    }
                                    else
                                    {
                                        szName = ws.Cells[i, nColName].Value.ToString().Trim();
                                    }
                                    if (ws.Cells[i, nColID].Value == null)
                                    {
                                        szID = "";
                                    }
                                    else
                                    {
                                        szID = ws.Cells[i, nColID].Value.ToString().Trim();
                                    }
                                    if ((szID == "" && szName == ""))
                                    {//都是空都不需要填写
                                        break;
                                    }
                                    string szAddress = getAddressStr(ws.Cells[i, nColName].Address);
                                    if (checkFunc(ref szID, ref szName, nUsefulIndex > 0 ? false : true, szAddress, ref rInfoOutput))
                                    {
                                        ws.Cells[i, nColName].Interior.ColorIndex = 0;
                                        ws.Cells[i, nColID].Interior.ColorIndex = 0;
                                    }
                                    else
                                    {//标黄
                                        ws.Cells[i, nColName].Interior.Color = Color.FromArgb(255, 255, 0);
                                        ws.Cells[i, nColID].Interior.Color = Color.FromArgb(255, 255, 0);
                                        isResultSucced = false;
                                    }
                                }
                            }
                        }
                        bIsHead = false;
                    }
                }
                if (isResultSucced)
                {
                    rInfoOutput.SelectionColor = Color.Green;
                    rInfoOutput.AppendText("检查通过 ！！！\n");
                }
            }
            catch (Exception ex)
            {
                rInfoOutput.SelectionColor = Color.Red;
                rInfoOutput.AppendText(ex.ToString() + '\n');
            }
            finally
            {
                excelCoverClose(szPath, excel, rWbk, ref rInfoOutput);
            }
        }
        public static void excelWrite(string szPath,  OperatorFunc rOperatorFunc, ref System.Windows.Forms.RichTextBox rInfoOutput)
        {
            string szExcelFilePath = szPath.Trim();
            Microsoft.Office.Interop.Excel.Application excel = new Microsoft.Office.Interop.Excel.Application();
            Workbooks wb = excel.Workbooks;
            excel.Visible = false;//设置调用引用的 Excel文件是否可见
            excel.Application.DisplayAlerts = false;
            //wb = excel.Workbooks.Open(ExcelFilePath);
            Workbook rWbk = wb.Add(szExcelFilePath);
            Sheets rWorkSheets = rWbk.Worksheets;
            try
            {//每个工作表都查 索引从1开始
                int[] rColNum = new int[(int)OperatorHead.HeadMax];
                for (int index = 1; index <= rWorkSheets.Count; ++index)
                {
                    Worksheet ws = (Worksheet)rWorkSheets[index];
                    int rowCount = 0;//有效行，索引从1开始
                    rowCount = ws.UsedRange.Rows.Count;//赋值有效行
                    bool bIsHead = false;
                    bool bIsEnd = true;
                    for (int i = 1; i <= rowCount; i++)//
                    {//将行中数据交给 代理处理
                        string[] rUseFulContent = new string[(int)OperatorHead.HeadMax];
                        int nColCount = ws.UsedRange.Columns.Count;
                        for (int nLoopCount = ws.UsedRange.Column; nLoopCount <= nColCount ;  ++nLoopCount)
                        {//循环一行中的每一列
                            if(ws.Cells[i, nLoopCount].Value == null)
                            {
                                if (nLoopCount == 1)
                                {
                                    bIsEnd = true;
                                }
                                else
                                {
                                    bIsEnd = bIsEnd && true;
                                }
                                continue;
                            }
                            else
                            {
                                if (nLoopCount == 1)
                                {
                                    bIsEnd = false;
                                }
                                else
                                {
                                    bIsEnd = bIsEnd && false;
                                }
                            }
                            string szContent = ws.Cells[i, nLoopCount].Value.ToString().Trim();
                            bIsEnd = bIsEnd && szContent == ""; 
                            if (szContent == "")
                            {
                                continue;
                            }
                            if ( checkIsHead(szContent))
                            {//代表当前行中存在表头
                                if(!bIsHead)
                                {
                                    bIsHead = true;
                                    clearHeadindexArr(ref rColNum);
                                }
                                //记录行列标记
                                setHeadindex(szContent, nLoopCount, ref rColNum);
                            }
                            string szAddress = ws.Cells[i, nLoopCount].Address;
                        }
                        if (bIsEnd)
                            {
                                clearHeadindexArr(ref rColNum);
                                continue;
                            }
                        if (!bIsHead && headValidaion(rColNum[0], rColNum[1], rColNum[2], rColNum[3]))
                        {
                          
                            for (int nUsefulIndex = 0; nUsefulIndex < rColNum.Length; nUsefulIndex += 2)
                            {
                                int nColName = rColNum[nUsefulIndex];
                                int nColID = rColNum[nUsefulIndex + 1];
                                if (0 != nColID && 0 != nColName)
                                {
                                    string szName;
                                    string szID;
                                    if (ws.Cells[i, nColName].Value == null)
                                    {
                                        szName = "";
                                    }
                                    else
                                    {
                                        szName = ws.Cells[i, nColName].Value.ToString().Trim();
                                    }
                                    if(ws.Cells[i, nColID].Value == null)
                                    {
                                        szID = "";
                                    }
                                    else
                                    {
                                        szID = ws.Cells[i, nColID].Value.ToString().Trim();
                                    }
                                    if((szID == "" && szName == "") || (szID != "" && szName != ""))
                                    {//都不是空，或者都是空都不需要填写

                                    }
                                    else
                                    {
                                        if (rOperatorFunc(ref szName, ref szID, nUsefulIndex > 0 ? false : true))
                                        {
                                            ws.Cells[i, nColName] = szName;
                                            ws.Cells[i, nColID] = szID;
                                        }
                                    }
                                }
                            }
                        }
                        bIsHead = false;
                    }
                }
            }
            catch (Exception ex)
            {
                rInfoOutput.SelectionColor = Color.Red;
                rInfoOutput.AppendText(ex.ToString() + '\n');
            }
            finally
            {
                excelSaveClose(szPath, excel, rWbk,ref rInfoOutput);
            }
        }

      
        private static void excelSaveClose( string szPath, Application rExcel,Workbook rWorkbook, ref System.Windows.Forms.RichTextBox rInfoOutput)
        {
            if (rWorkbook != null)
            {
                //rExcel.Visible = false;
                try
                {
                    //保存结果
                    rWorkbook.Close(true, Type.Missing, Type.Missing);
                }
                catch(Exception ex)
                {
                    rInfoOutput.SelectionColor = Color.Red;
                    rInfoOutput.AppendText(ex.ToString() + '\n');
                }
                finally
                {
                    rExcel.Quit();
                    // 安全回收进程
                    System.GC.GetGeneration(rExcel);
                }
            }
        }
        private static void excelCoverClose(string szPath, Microsoft.Office.Interop.Excel.Application rExcel, Workbook rWorkbook,ref System.Windows.Forms.RichTextBox rInfoOutput)
        {
            Process[] localByNameApp = Process.GetProcessesByName(szPath);//获取程序名的所有进程
            if (localByNameApp.Length > 0)
            {
                foreach (var app in localByNameApp)
                {
                    #region
                    ////设置禁止弹出保存和覆盖的询问提示框   
                    rExcel.DisplayAlerts = false;
                    rExcel.AlertBeforeOverwriting = false;
                    rExcel.Visible = false;
                    //wb.Saved = true;
                    ////保存工作簿   
                    rExcel.Application.Workbooks.Add(true).Save();
                    //保存excel文件   
                    ///excel.Save("E:\\c#_test\\winFormTest\\winFormTest\\hahaha.xls");
                    //确保Excel进程关闭   
                    rExcel.Quit();
                    rExcel = null;
                    #endregion
                    app.Kill();//关闭进程  
                }
            }
            if (rWorkbook != null)
            {
                try
                {
                    rExcel.DisplayAlerts = false;
                    rExcel.AlertBeforeOverwriting = false;
                    rExcel.Visible = false;
                    /*rExcel.Application.Workbooks.Add(true).Save();*/
                    //保存结果
                    rWorkbook.SaveAs(szPath, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing, XlSaveAsAccessMode.xlNoChange, Type.Missing, Type.Missing, Type.Missing);
                    rWorkbook.Save();
                    rWorkbook.Close(false, szPath, szPath);
                }
                catch(Exception ex)
                {
                    rInfoOutput.SelectionColor = Color.Red;
                    rInfoOutput.AppendText(ex.ToString() + '\n');
                }
                finally
                {
                    rExcel.Quit();

                }
            }
            // 安全回收进程
            System.GC.GetGeneration(rExcel);
        }
        private static string getAddressStr(string szAddress)
        {
            string[] tempSz = szAddress.Split('$');
            string szResult = null;
            for(int index = 0;index < tempSz.Length;++index)
            {
                if(szResult == null)
                {
                    szResult = tempSz[index];
                }
                else
                {
                    szResult += tempSz[index];
                }
            }
            return szResult;
        }
        private static bool checkIsHead(string szContent)
        {
            for (int index = 0; index < rHeadArr.Length; ++index)
            {
                if (szContent == rHeadArr[index])
                {
                    return true;
                }
            }
            return false;
        }
        private static void setHeadindex(string szContent, int col, ref int[] nColNum)
        {
            for (int index = 0; index < rHeadArr.Length && index < nColNum.Length; ++index)
            {
                if (szContent == rHeadArr[index])
                {
                    nColNum[index] = col;
                }
            }
        }
        private static void clearHeadindexArr(ref int[] rColNum)
        {
            for (int index = 0; index < rColNum.Length; ++index)
            {
                rColNum[index] = 0;
            }
        }
        private static bool headValidaion(int nManCol, int nManID, int nWomanCol, int nWomanID)
        {
            bool bMan = (nManCol != 0 && nManID != 0);
            bool bWoman = (nWomanCol != 0 && nWomanID != 0);
            return bMan || bWoman;
        }
    }
}
