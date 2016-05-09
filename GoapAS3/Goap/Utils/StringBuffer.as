package Goap.Utils
{
	public class StringBuffer
	{
		protected var buffer:Array = new Array();
		
		public function add(str:String):void
		{
			for (var i:Number = 0; i < str.length; i++)
			{
				buffer.push(str.charCodeAt(i));
			}
		}
		
		public function clear():void
		{
			buffer.length = 0;
		}
		
		public function toString():String
		{
			return String.fromCharCode.apply(this, buffer);
		}
	}
}