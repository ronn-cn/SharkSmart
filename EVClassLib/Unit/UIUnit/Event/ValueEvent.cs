using System;
using System.ComponentModel;

namespace EVClassLib
{
    [Serializable]
    public class ValueEvent : BaseEvent
    {
        [DisplayName("值改变事件"), Category("拓展事件"), Description("值改变事件")]
        public string ValueChanged { set; get; }
    }
}
