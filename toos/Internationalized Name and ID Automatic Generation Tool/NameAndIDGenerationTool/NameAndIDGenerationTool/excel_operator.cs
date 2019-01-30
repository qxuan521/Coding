using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Microsoft.Office.Interop.Excel;
using System.Diagnostics;
using System.Drawing;

namespace NameAndIDGenerationTool
{
    public delegate bool SaveData(string szID,string szName);

    class ExcelOperator
    {
        public static bool checkSrcExist(string szFileName)
        {
            return File.Exists(@szFileName);
        }

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
                        rInfoOutput.AppendText(strReadline + '\n');
                    }
                }
                rReader.Close();
                rFile.Close();
            }
            catch (Exception ex)
            {
                rInfoOutput.SelectionColor = Color.Red;
                rInfoOutput.AppendText(ex.ToString());
                rReader.Close();
                rFile.Close();
            }
            finally
            {
                /*ExcelClose(szPath, excel, rWbk);*/
            }
            return ;
        }

        public static bool comReaderNSaveData(string szPath, SaveData rSaveFunc,ref System.Windows.Forms.RichTextBox rInfoOutput)
        {
            string szExcelFilePath = szPath.Trim();
            Excel.Application excel = new Excel.Application();
            Excel.Workbooks wb = excel.Workbooks;
            excel.Visible = false;//设置调用引用的 Excel文件是否可见
            excel.Application.DisplayAlerts = false;
            //wb = excel.Workbooks.Open(ExcelFilePath);
            Excel.Workbook rWbk = wb.Add(szExcelFilePath);
            Excel.Sheets rWorkSheets = rWbk.Worksheets;
            try
            {//每个工作表都查 索引从1开始
                for (int index = 1; index <= rWorkSheets.Count; ++index)
                {
                    Excel.Worksheet ws = (Excel.Worksheet)rWorkSheets[index];
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

        public static void write_temp_file(ref Dictionary<string,string> rMap)
        {
            FileStream rFile = new FileStream("aaaaa.csv", FileMode.OpenOrCreate, FileAccess.Write);
            StreamWriter rReader = new StreamWriter(rFile, Encoding.Default);
            foreach (var temp in rMap)
            {
                rReader.WriteLine(temp.Key + " , " + temp.Value);
            }
            rReader.Close();
            rFile.Close();
        }
    }
}
