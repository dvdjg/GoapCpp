package Goap
{
	import flash.utils.ByteArray;

	public final class Hashing
	{
		public var name:String;
		public var value:*;
		
		public static var pool:ObjectPool = ObjectPool.getInstance(Hashing);
		[Inline]
		public final function dispose():void
		{
			pool.dispose(this);
		}
		
		public static function getNew(name_:String, value_:*):Hashing
		{
			var pair:Hashing = pool.getNew();
			pair.name = name_;
			pair.value = value_;
			return pair;
		}
		
		public static function hashObject(o:*):uint
		{
			return hashObject_AP(o);
		}
		
		private static var _sorted:Array = [];
		private static function getSortedPairs(object:Object):Array
		{
			for each(var sorted:Hashing in _sorted)
				sorted.dispose();
			_sorted.length = 0;
			for(var i:String in object) {
				var val:* = object[i];
				if (val != null) // Null values are the same as the entry does not exists
					_sorted.push(getNew(i, val));
			}
			_sorted.sortOn("name");
			return _sorted;
		}
		
		private static var _bytes:ByteArray = new ByteArray;
		public static function hashObject_AP(o:*):uint
		{
			var arr:Array = getSortedPairs(o);
			_bytes.position = 0;
			for each(var obj:Hashing in arr) {
				_bytes.writeUTFBytes(obj.name);
				_bytes.writeUTFBytes(obj.value);
			}
			_bytes.length = _bytes.position;
			return hash_AP(_bytes);
		}
		
		public static function hash_AP(bytes:ByteArray, seed:uint=0xAAAAAAAB):uint
		{
			var hash:uint=seed;
			var tmp:uint;
			var i:uint;
			var remaining:uint = 3 & (4 - (bytes.length & 3));
			bytes.position = bytes.length;
			while (remaining--) {
				bytes.writeByte(0);
			}
//			var oldLength:uint = bytes.length;
//			var newLength:uint = (bytes.length + 3) & ~3;
//			bytes.length = newLength;
			bytes.position = 0;
			while (bytes.bytesAvailable)
			{
				tmp=bytes.readUnsignedInt();
				if ((i++ & 1) == 0) {
					hash^=((hash << 7) ^ tmp * (hash >> 3));
				} else {
					hash^=(~((hash << 11) + tmp ^ (hash >> 5)));
				}
			}
			if (hash == 0) {
				hash = 0xAAAAAAAB;
			}
			
			return hash;
		}
	}
}