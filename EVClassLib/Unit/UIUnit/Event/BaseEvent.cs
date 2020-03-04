using System;
using System.ComponentModel;

namespace EVClassLib
{
    [Serializable]
    public class BaseEvent
    {
        [DisplayName("点击事件"), Category("基础事件"), Description("点击事件")]
        public string Clicked { set; get; }

        [DisplayName("释放事件"), Category("基础事件"), Description("释放事件")]
        public string Released { set; get; }

        [DisplayName("指针移除事件"), Category("基础事件"), Description("指针移除事件")]
        public string MovedOut { set; get; }

        public BaseEvent()
        {

        }
    }
}
