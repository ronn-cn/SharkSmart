using System;
using System.Collections.Generic;

namespace EVClassLib
{
    interface IRecord
    {
        void AddProjectRecord(HistoryProjectRecord record);

        void UpdateProjectRecord(HistoryProjectRecord record);

        void DeleteProjectRecord(Guid ID, bool IsDeleteFolder);

        List<HistoryProjectRecord> GetProjectRecords();
    }
}
