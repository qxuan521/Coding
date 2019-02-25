using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;
using System.Drawing;
using Microsoft.Office.Interop.Excel;

namespace X51Tools.TopicPictureDemandGenerationTool
{
    public delegate bool TableRowWriteFunc(ref string[] rRowData);
    class TopicExcelOperator : Global.ExcelOperator
    {
        //brief
        //继承自ExcelOperator
        //提供主题配置需求初版excel表格的填写支持逻辑
        //
        public void excelFillIn(string szPath, TableRowWriteFunc rOperatorFunc,  ref string szError)
        {
            //打开excel
            openExcel(szPath);
            Workbook rWorkbook = getWorkbook();
            Sheets rWorkSheets = rWorkbook.Worksheets;
            bool rNoError = true;
            try
            {//每个工作表都查 索引从1开始
                for (int index = 1; index <= rWorkSheets.Count; ++index)
                {
                    Worksheet rWs = (Worksheet)rWorkSheets[index];
                    if (rWs.Name == "系列套装配置")
                    {//系列套装配置表
                        int[] rValidColArr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
                        rNoError = TableFillIn(rValidColArr, rOperatorFunc, rWs) && rNoError;
                    }
                    else if (rWs.Name == "系列套装部件产出途径文本预设及系列碎片产出文本")
                    {//表
                        int[] rValidColArr = { 1,2,4,5 };
                        rNoError = TableFillIn(rValidColArr, rOperatorFunc, rWs) && rNoError;
                    }
//                     else if (rWs.Name == "额外奖励配置")
//                     {//额外奖励配置表
//                         int[] rValidColArr = { 1, 2, 3, 4, 5, 6};
//                         rNoError = TableFillIn(rValidColArr, rOperatorFunc, rWs) && rNoError;
//                     }
                }
                //xmlFunc
            }
            catch (Exception ex)
            {
                szError = ex.ToString() + "\n";
            }
            finally
            {
                //关闭excel
            }
        }
        //
        // 获得数据行数
        //
        public int getValidDataRowCount(string szPageName)
        {
            Workbook rWorkbook = getWorkbook();
            Sheets rWorkSheets = rWorkbook.Worksheets;
            for (int index = 1; index <= rWorkSheets.Count; ++index)
            {
                Worksheet rWs = (Worksheet)rWorkSheets[index];
                if (szPageName == rWs.Name)
                {
                    return rWs.UsedRange.Rows.Count - 1;
                }
            }
            return -1;
        }
        //
        // 表格行读取函数
        //
        public bool getExcelRow(ref string[] rRowData, int nRowNum,string szPageName)
        {
            Workbook rWorkbook = getWorkbook();
            Sheets rWorkSheets = rWorkbook.Worksheets;
            try
            {//每个工作表都查 索引从1开始
                for (int index = 1; index <= rWorkSheets.Count; ++index)
                {
                    Worksheet rWs = (Worksheet)rWorkSheets[index];
                    if(szPageName == rWs.Name)
                    {
                        int[] rTempArr;
                        if (rWs.Name == "系列套装配置")
                        {//系列套装配置表
                            int[] rValidColArr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
                            rTempArr = rValidColArr;
                        }
                        else if (rWs.Name == "系列套装部件产出途径文本预设及系列碎片产出文本")
                        {//表
                            int[] rValidColArr = { 1, 2, 4, 5 };
                            rTempArr = rValidColArr;
                        }
                        else if(rWs.Name == "额外奖励配置")
                        {//额外奖励配置表
                            int[] rValidColArr = { 1, 2, 3, 4, 5, 6 };
                            rTempArr = rValidColArr;
                        }
                        else
                        {
                            return false;
                        }
                        if (rWs.UsedRange.Rows.Count >= nRowNum)
                        {
                            rRowData = getRowValidData(rTempArr, nRowNum, rWs);
                            return true;
                        }
                    }
                }
                //xmlFunc
            }
            catch (Exception ex)
            {
                string szError = ex.ToString();
            }
            finally
            {
                //关闭excel
            }
            return false;
        }
        //
        //读取无优惠表格
        //
        public void readNoSaleTable(string szPath,ref Dictionary<string,int> rNosale)
        {
            openExcel(szPath);
            Workbook rWorkbook = getWorkbook();
            Sheets rWorkSheets = rWorkbook.Worksheets;
            try
            {//每个工作表都查 索引从1开始
                for (int index = 1; index <= rWorkSheets.Count; ++index)
                {
                    Worksheet rWs = (Worksheet)rWorkSheets[index];
                    for (int nRowNum = 1; nRowNum <= rWs.UsedRange.Rows.Count; ++nRowNum)
                    {
                        if (rWs.Cells[nRowNum, 1].Value == null)
                        {
                            return;
                        }
                        else
                        {
                            rNosale.Add(rWs.Cells[nRowNum, 1].Value.ToString(),1);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
            }
            finally
            {
                //关闭excel
                closeExcel();
            }
        }
        //
        //读取查询列表
        //
        public void readSearchListTable(string szPath, ref Dictionary<string, bool> rSearchList, ref Dictionary<string, bool> rNosale)
        {
            openExcel(szPath);
            Workbook rWorkbook = getWorkbook();
            Sheets rWorkSheets = rWorkbook.Worksheets;
            try
            {//每个工作表都查 索引从1开始
                bool bCol1Over = false;
                bool bCol2Over = false;
                for (int index = 1; index <= rWorkSheets.Count; ++index)
                {
                    Worksheet rWs = (Worksheet)rWorkSheets[index];
                    for (int nRowNum = 1; nRowNum <= rWs.UsedRange.Rows.Count; ++nRowNum)
                    {
                        if(!bCol1Over)
                        {
                            if (rWs.Cells[nRowNum, 1].Value == null)
                            {
                                bCol1Over = true;
                            }
                            else
                            {
                                string szData = rWs.Cells[nRowNum, 1].Value.ToString();
                                if (rSearchList.Count == 0)
                                {
                                    rSearchList.Add(szData, false);
                                }
                                else
                                {
                                    if (!rSearchList.ContainsKey(szData))
                                    {
                                        rSearchList.Add(szData, false);
                                    }
                                }
                                
                            }
                        }
                        if(!bCol2Over)
                        {
                            if (rWs.Cells[nRowNum, 2].Value == null)
                            {
                                bCol2Over = true;
                            }
                            else
                            {
                                if (!rNosale.ContainsKey(rWs.Cells[nRowNum, 2].Value.ToString()))
                                {
                                    rNosale.Add(rWs.Cells[nRowNum, 2].Value.ToString(), false);
                                }
                            }
                        }
                        if(bCol2Over && bCol1Over)
                        {
                            return;
                        }
                        
                    }
                }
            }
            catch (Exception ex)
            {
                string szError = ex.ToString();
            }
            finally
            {
                //关闭excel
                closeExcel();
            }
        }
        //
        //private func
        //

        private bool TableFillIn(int[] rValidColArr, TableRowWriteFunc rOperatorFunc, Worksheet rWs)
        {
            bool bResult = true;
            for (int nRowNum = 2; nRowNum <= rWs.UsedRange.Rows.Count; ++nRowNum)
            {
                string[] rValidStrArr = getRowValidData(rValidColArr, nRowNum, rWs);
                bResult = rOperatorFunc(ref rValidStrArr) && bResult;
                setRowValidData(rValidColArr, nRowNum, rWs,ref rValidStrArr);
            }
            return bResult;
        }
        private string[] getRowValidData(int[] rValidArr, int nRow, Worksheet rWs)
        {
            int nColCount = rWs.UsedRange.Columns.Count;
            string[] rResultArr = new string[rValidArr.Length];
            for (int nLoopCount = 0; nLoopCount < rValidArr.Length && rValidArr[nLoopCount] <= nColCount; ++nLoopCount)
            {//循环一行中的每一列
                if (rWs.Cells[nRow, rValidArr[nLoopCount]].Value == null)
                {
                    rResultArr[nLoopCount] = "";
                }
                else
                {
                    rResultArr[nLoopCount] = rWs.Cells[nRow, rValidArr[nLoopCount]].Value.ToString();
                }
            }
            return rResultArr;
        }

        private void setRowValidData(int[] rValidArr, int nRow, Worksheet rWs, ref string[] rValidStrArr)
        {
            int nColCount = rWs.UsedRange.Columns.Count;
            for (int nLoopCount = 0; nLoopCount < rValidArr.Length && rValidArr[nLoopCount] <= nColCount; ++nLoopCount)
            {//循环一行中的每一列
                if (rWs.Cells[nRow, rValidArr[nLoopCount]].Value == null)
                {
                    rWs.Cells[nRow, rValidArr[nLoopCount]].Value = rValidStrArr[nLoopCount];
                }
                else
                {
                    //rWs.Cells[nRow, rValidArr[nLoopCount]].Value = rValidStrArr[nLoopCount];
                }
            }
        }
       

    }
}