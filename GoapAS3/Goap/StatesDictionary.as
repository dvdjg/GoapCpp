package Goap
{
	import flash.utils.Dictionary;

	/**
	 * Assumes that two different States has different Hashes
	 */
	public final class StatesDictionary
	{
		private var _dictionary:Dictionary = new Dictionary;
		
		[Inline]
		public final function at(key:IReadState):*
		{
			var value:* = _dictionary[key.hash];
			return value;
		}
		
		[Inline]
		public final function setAt(key:IReadState, value:*):void
		{
			var val:* = _dictionary[key.hash];
			if (val != null) {
				trace("[StatesDictionary] Overwritting.");
			}			
			_dictionary[key.hash] = value;
		}
		
		[Inline]
		public final function remove(key:IReadState):void
		{
			delete _dictionary[key.hash];
		}
		
		public function get length():int
		{
			var ret:int = 0;
			for (var k:* in _dictionary) {
				++ret;
			}
			return ret;
		}
		
		public final function clear():void
		{
			for (var k:* in _dictionary) {
				delete _dictionary[k];
			}
		}
		
		[Inline]
		public final function get isEmpty():Boolean
		{
			for (var k:* in _dictionary) {
				return false;
			}
			return true;
		}
	}
}