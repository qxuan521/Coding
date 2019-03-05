using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Office.Interop.Excel;
using System.Diagnostics;
namespace X51Tools.Global
{
    class ExcelOperator
    {
        //property
        private Microsoft.Office.Interop.Excel.Application m_rExcel = null;
        private Microsoft.Office.Interop.Excel.Workbook m_rWorkBook;
        private string m_szFilePath;
        private bool m_isDismiss = false;
        //function
        ~ExcelOperator()
        {
            if(!m_isDismiss && null != m_rExcel)
            {
                System.GC.GetGeneration(m_rExcel);
                m_rExcel = null;
            }
        }
        public void openExcel(string szExcelFilePath)
        {
            m_isDismiss = false;
            m_rExcel = new Microsoft.Office.Interop.Excel.Application();
            m_rExcel.Visible = false;//设置调用引用的 Excel文件是否可见
            m_rExcel.Application.DisplayAlerts = false;
            //wb = excel.Workbooks.Open(ExcelFilePath);
            m_rWorkBook = m_rExcel.Workbooks.Add(szExcelFilePath);
            m_szFilePath = szExcelFilePath;
        }
        public void createNewExcel()
        {
            m_isDismiss = false;
            m_rExcel = new Microsoft.Office.Interop.Excel.Application();
            m_rExcel.Visible = false;//设置调用引用的 Excel文件是否可见
            m_rExcel.Application.DisplayAlerts = false;
            //wb = excel.Workbooks.Open(ExcelFilePath);
            m_rWorkBook = m_rExcel.Workbooks.Add(true);
        }
        //覆盖保存
        public void saveCover()
        {
            Process[] localByNameApp = Process.GetProcessesByName(m_szFilePath);//获取程序名的所有进程
            if (localByNameApp.Length > 0)
            {
                foreach (var app in localByNameApp)
                {
                    #region
                    ////设置禁止弹出保存和覆盖的询问提示框   
                    m_rExcel.DisplayAlerts = false;
                    m_rExcel.AlertBeforeOverwriting = false;
                    m_rExcel.Visible = false;
                    ////保存工作簿   
                    m_rExcel.Application.Workbooks.Add(true).Save();
                    //保存excel文件   
                    ///excel.Save("E:\\c#_test\\winFormTest\\winFormTest\\hahaha.xls");
                    //确保Excel进程关闭   
                    m_rExcel.Quit();
                    m_rExcel = null;
                    #endregion
                    app.Kill();//关闭进程  
                }
            }
            if (m_rWorkBook != null)
            {
                try
                {
                    m_rExcel.DisplayAlerts = false;
                    m_rExcel.AlertBeforeOverwriting = false;
                    m_rExcel.Visible = false;
                    //保存结果
                    m_rWorkBook.SaveAs(m_szFilePath, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing, XlSaveAsAccessMode.xlNoChange, Type.Missing, Type.Missing, Type.Missing);
                    m_rWorkBook.Save();
                    m_rWorkBook.Close(false, m_szFilePath, m_szFilePath);
                }
                catch (Exception ex)
                {
                    ex.ToString();
                }
                finally
                {
                    m_rExcel.Quit();
                }
            }
            // 安全回收进程
            dismiss();
            System.GC.GetGeneration(m_rExcel);
            m_rExcel = null;
            m_szFilePath = "";
        }
        public void closeExcel()
        {
            Process[] localByNameApp = Process.GetProcessesByName(m_szFilePath);//获取程序名的所有进程
            if (localByNameApp.Length > 0)
            {
                foreach (var app in localByNameApp)
                {
                    #region
                    ////设置禁止弹出保存和覆盖的询问提示框   
                    m_rExcel.DisplayAlerts = false;
                    m_rExcel.AlertBeforeOverwriting = false;
                    m_rExcel.Visible = false;
                    ////保存工作簿   
                    //确保Excel进程关闭   
                    m_rExcel.Quit();
                    m_rExcel = null;
                    #endregion
                    app.Kill();//关闭进程  
                }
            }
            if (m_rWorkBook != null)
            {
                try
                {
                    m_rExcel.DisplayAlerts = false;
                    m_rExcel.AlertBeforeOverwriting = false;
                    m_rExcel.Visible = false;
                    //保存结果
                }
                catch (Exception ex)
                {
                    ex.ToString();
                }
                finally
                {
                    m_rExcel.Quit();
                }
            }
            // 安全回收进程
            dismiss();
            System.GC.GetGeneration(m_rExcel);
            m_rExcel = null;
            m_szFilePath = "";
        }
        //另存为
        public void saveAsNewFile()
        {
            if (m_rWorkBook != null)
            {
                //rExcel.Visible = false;
                try
                {
                    //保存结果
                    m_rWorkBook.Close(true, Type.Missing, Type.Missing);
                }
                catch(Exception ex)
                {
                    ex.ToString();
                }
                finally
                {
                    m_rExcel.Quit();
                    dismiss();
                    // 安全回收进程
                    System.GC.GetGeneration(m_rExcel);
                    m_rExcel = null;
                    m_szFilePath = "";
                }
            }
        }
        public Workbook getWorkbook()
        {
            return m_rWorkBook;
        }
        //private
        private void dismiss()
        {
            m_isDismiss = true;
        }
    }
}
