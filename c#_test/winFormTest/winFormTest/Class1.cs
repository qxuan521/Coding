using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Office.Interop.Excel;
using System.Diagnostics;
using System.Drawing;
namespace winFormTest
{
    class ExcelOperator
    {
        public void ExcelOp(string szPath)
        {
            string ExcelFilePath = szPath.Trim();
            //set columns
            Dictionary<string, string> dic = new Dictionary<string, string>();
            dic.Add("物品男", "A");//
            dic.Add("id男", "B");
            Excel.Application excel = new Excel.Application();
            Excel.Workbooks wb = excel.Workbooks;
            excel.Visible = false;//设置调用引用的 Excel文件是否可见
            excel.Application.DisplayAlerts = false;
            //wb = excel.Workbooks.Open(ExcelFilePath);
            Excel.Workbook rWbk = wb.Add(szPath);
            Excel.Worksheet ws = (Excel.Worksheet)rWbk.Worksheets[1]; //索引从1开始 //(Excel.Worksheet)wb.Worksheets["SheetName"];
            int rowCount = 0;//有效行，索引从1开始
            try
            {
                rowCount = ws.UsedRange.Rows.Count;//赋值有效行
                int columns = ws.UsedRange.Columns.Count;
                string ordernum = string.Empty;
                string count = string.Empty;
                //循环行
                for (int i = 1; i <= 10; i++)//
                {
                    if (ws.Rows[i] != null)
                    {
                        for (int j = 1; j <= 10; j++)
                        {
                            ws.Cells[i, j].value = "aaaa";
                            string szPos = "$";
                            szPos += i;
                            string szCol = "$";
                            szCol += j;
                            string cellStr = szPos + ":" + szCol;
                            string szId = ws.Cells.get_Address(i, j);
                            //ws.get_Range(cellStr, cellStr).Interior.Color = Color.FromArgb(0,111, 111, 255);
                            // ((Range)ws.Cells[i, j]).Interior.Color = Color.FromArgb(0, 255, 255, 255);
                            ((Range)ws.Cells[i, j]).Interior.ColorIndex = 0;
                        }
                    }
                }
            }
            catch (Exception ex) { }
            finally
            {
                ExcelClose(szPath, excel, rWbk);
            }
        }
        public void ExcelClose(string szPath, Excel.Application excel, Excel.Workbook wb)
        {
            Process[] localByNameApp = Process.GetProcessesByName(szPath);//获取程序名的所有进程
            if (localByNameApp.Length > 0)
            {
                foreach (var app in localByNameApp)
                {
//                  if (!app.HasExited)
//                  {
                    #region
                    ////设置禁止弹出保存和覆盖的询问提示框   
                    excel.DisplayAlerts = false;
                    excel.AlertBeforeOverwriting = false;
                    excel.Visible = false;
                    //wb.Saved = true;
                    ////保存工作簿   
                    excel.Application.Workbooks.Add(true).Save();
                    //保存excel文件   
                    ///excel.Save("E:\\c#_test\\winFormTest\\winFormTest\\hahaha.xls");
                    //确保Excel进程关闭   
                    excel.Quit();
                    excel = null;
                    #endregion
                    app.Kill();//关闭进程  
                    /*}*/
                }
            }
            if (wb != null)
            {
                excel.DisplayAlerts = false;
                excel.AlertBeforeOverwriting = false;
                excel.Visible = false;
                excel.Application.Workbooks.Add(true).Save();
                //保存结果
                wb.SaveAs(szPath, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Excel.XlSaveAsAccessMode.xlNoChange,Type.Missing, Type.Missing, Type.Missing);
                //wb.Save();
                wb.Close(false, szPath, szPath);
               
            }
            excel.Quit();
            // 安全回收进程
            System.GC.GetGeneration(excel);
        }
    }
}
