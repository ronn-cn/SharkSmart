using System;
using System.ComponentModel;

namespace EVClassLib
{
    [Serializable]
    public class ListEvent : BaseEvent
    {
        [DisplayName("滚动条改变事件"), Category("拓展事件"), Description("滚动条改变事件")]
        public string ScrollChanged { set; get; }

        [DisplayName("小工具值改变事件"), Category("拓展事件"), Description("小工具值改变事件")]
        public string SelChanged { set; get; }
    }
}
