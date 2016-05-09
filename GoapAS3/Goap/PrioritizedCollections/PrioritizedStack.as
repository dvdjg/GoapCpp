package Goap.PrioritizedCollections
{
	import Goap.Graph.Path;

	/**
	 * @author David
	 */
	public class PrioritizedStack extends PrioritizedQueue
	{
		/**
		 * The path is inserted in the processing list. 
		 * If its cost is lower than current level it will be executed the next. 
		 */
		public override function pop():Path
		{			
			if (isEmpty) 
				return null;
		
			// Takes the minimum cost queue 
			var min:int = _min;
			var queue:Array = _queues[min];
			var value:Path = queue.pop();
			if (value == null) {
				throw new Error("[PrioritizedQueue] pop error");
			}
			if (queue.length == 0) {
				// Use the lazy array when current buffer has finished
				useLazyArray();
				if (queue.length == 0) {
					// If after reinserting elements this buffer continues empty, then delete it
					delete _queues[min];
					if (min == _min) {
						// The min value is invalid, so we need to recompute it
						_min = 0x7FFFFFFF;
						for (var key:int in _queues) 
							if (key < _min)
								_min = key;
					}
				}
			}
			return value;
		}
	}	
}
