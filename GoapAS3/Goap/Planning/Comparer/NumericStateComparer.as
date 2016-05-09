package Goap.Planning.Comparer
{
	import Goap.IReadState;

	/**
	 * Compare states
	 */
	public class NumericStateComparer implements IPlanningStateComparer
	{
		private static var _singleton:NumericStateComparer = new NumericStateComparer;
		[Inline]
		public static function get singleton():NumericStateComparer 
		{
			return _singleton;
		}
		
		/** 
		 * Gives an idea of how far are two states one to another. If Distance == 0, both are equal
		 * A percentage numeric distance is used to compute de similarities.
		 * Both states must have numerical values.
		 * Both have the same state if s1 has at least the same fields than s2. Anyway, s1 can have more states.
		 * Only the common keys are compared.
		 */
		public function distance(s1:IReadState, s2:IReadState):Number
		{
			var percent:Number = 0;
			var countDifferent:Number = 0;
			var s2Count:int = 0;
			var sameKeyCount:int = 0; // Counts the number of keys that s1 has like s2
			for (var key:String in s2.getMap()) {
				var value1:* = s1.at(key);
				var value2:*;
				if (value1 is Number || value1 is int) { // Use a numeric distance function
					var s1k:Number = value1;
					value2 = s2.at(key);
					var s2k:Number = value2;
					sameKeyCount++;
					if (s1k != s2k) {
						// 0 < are similar < are different < 1 
						percent += Math.abs(s1k - s2k) / Math.max(s1k, s2k);
					}
				} else if (value1 != null) {
					sameKeyCount++;
					value2 = s2.at(key);
					if (value1 != value2) { // This can compare strings, booleans, ...
						percent += 1; // Consider as different values
					}
				}
				++s2Count;
			}
			countDifferent = s2Count - sameKeyCount;
			return (percent + countDifferent) / s2Count;
		}
		
		/**
		 * s2 is the goal state. Test if the common keys are equal.
		 */
		public function enough(s1:IReadState, s2:IReadState):Boolean
		{
			for (var key:String in s2.getMap()) {
				var value1:* = s1.at(key);
				var value2:* = s2.at(key);
				if (value1 != value2) {
					return false;
				}
			}
			return true;
		}

	}	
}
