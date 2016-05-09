package Goap.Planning.Comparer
{
	import Goap.IReadState;

	/**
	 * @author David
	 */
	public class ExactStateComparer extends NumericStateComparer
	{
		private static var _singleton:ExactStateComparer = new ExactStateComparer;
		[Inline]
		public static function get singleton():ExactStateComparer 
		{
			return _singleton;
		}
		/** 
		 * Gives an idea of how far are two states one to another. If Distance == 0, both are equal
		 * A percentage exact state members count is used to compute de similarities.
		 * Both states can be of any type. The != operator is used to find similarities
		 */
		public function distancePure(s1:IReadState, s2:IReadState):Number
		{
			var percent:Number = 0;
			var s2Count:int = 0;
			for (var key:String in s2.getMap()) {
				var value1:* = s1.at(key);
				var value2:* = s2.at(key);
				if (value1 != value2) {
					percent += 1.0;
				}
				++s2Count;
			}
			return percent / s2Count;
		}
		
		/** 
		 * Gives an idea of how far are two states one to another. If Distance == 0, both are equal
		 * A percentage exact state members count is used to compute de similarities.
		 * Both states can be of any type. The != operator is used to find similarities
		 */
		public override function distance(s1:IReadState, s2:IReadState):Number
		{
			var percent:Number = 0;
			var countDifferent:Number = 0;
			var s2Count:int = 0;
			var sameKeyCount:int = 0;
			for (var key:String in s2.getMap()) {
				var value1:* = s1.at(key);
				if (value1 != null) {
					var value2:* = s2.at(key);
					sameKeyCount++;
					if (value2 != value1) {
						percent += 1;
					}
				}
				++s2Count;
			}
			countDifferent = s2Count + s1.length - sameKeyCount - sameKeyCount;
			return (percent + countDifferent) / (sameKeyCount + countDifferent);
		}
	}	
}
