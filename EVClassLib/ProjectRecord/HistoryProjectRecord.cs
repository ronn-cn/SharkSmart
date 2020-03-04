using System;

namespace EVClassLib
{
    /// <summary>
    /// 历史记录
    /// </summary>
    public class HistoryProjectRecord
    {
        public Guid ProjectID { set; get; }

        public DateTime Time { set; get; }

        public string ProjectPath { set; get; }
    }
}
