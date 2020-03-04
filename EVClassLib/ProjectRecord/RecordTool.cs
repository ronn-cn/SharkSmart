using EVTechnology.Common.Helper;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace EVClassLib
{
    public class RecordTool : IRecord
    {
        public void AddProjectRecord(HistoryProjectRecord record)
        {
            List<HistoryProjectRecord> list = new List<HistoryProjectRecord>();
            string filePath = Path.Combine(Environment.CurrentDirectory, SoftwareSetting.RecordPath);
            if (File.Exists(filePath))
            {
                list = FileHelper.AnalysisSerializeFile<List<HistoryProjectRecord>>(filePath, FileType.XMLFile);
            }
            list.Add(record);
            if (list.Count > SoftwareSetting.MaxRecordCount)
                list.RemoveAt(0);
            FileHelper.CreateSerializeFile(filePath, list, FileType.XMLFile);
        }

        public void DeleteProjectRecord(Guid ID, bool IsDeleteFolder)
        {
            List<HistoryProjectRecord> list = new List<HistoryProjectRecord>();
            string path = Path.Combine(Environment.CurrentDirectory, SoftwareSetting.RecordPath);
            if (File.Exists(path))
            {
                list = FileHelper.AnalysisSerializeFile<List<HistoryProjectRecord>>(path, FileType.XMLFile);
                HistoryProjectRecord record = list.Find(p => p.ProjectID == ID);
                list.Remove(record);
                FileHelper.CreateSerializeFile(path, list, FileType.XMLFile);

                if (IsDeleteFolder)
                {
                    FileHelper.CmdDeleteFolder(Path.GetDirectoryName(record.ProjectPath));
                }
            }
        }

        public List<HistoryProjectRecord> GetProjectRecords()
        {
            List<HistoryProjectRecord> list = new List<HistoryProjectRecord>();
            string path = Path.Combine(Environment.CurrentDirectory, SoftwareSetting.RecordPath);
            if (File.Exists(path))
            {
                list = FileHelper.AnalysisSerializeFile<List<HistoryProjectRecord>>(path, FileType.XMLFile);
            }
            list = list.OrderByDescending(p => p.Time).ToList();
            return list;
        }

        public void UpdateProjectRecord(HistoryProjectRecord record)
        {
            string filePath = Path.Combine(Environment.CurrentDirectory, SoftwareSetting.RecordPath);
            List<HistoryProjectRecord> records = GetProjectRecords();
            HistoryProjectRecord tmp = records.Find(p => p.ProjectID == record.ProjectID);
            if (tmp != null)
            {
                tmp.ProjectPath = record.ProjectPath;
                tmp.Time = record.Time;
                FileHelper.CreateSerializeFile(filePath, records, FileType.XMLFile);
            }
            else
                AddProjectRecord(record);
        }
    }
}
