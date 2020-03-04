using System;
using System.Collections.Generic;

namespace EVClassLib
{
    [Serializable]
    public class ReadOnlyModule : Module
    {
        public Dictionary<int, string> Setting { set; get; }
    }
}
