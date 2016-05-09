package Goap.PrioritizedCollections
{
	import flash.utils.Dictionary;
	
	import Goap.Graph.Path;

	/**
	 * @author David
	 */
	public class PrioritizedQueue implements IPrioritized
	{
		protected var _queues:Dictionary = new Dictionary;
		protected var _min:int = 0x7FFFFFFF;
		protected var _lazyArray:Array = [];
		
		public function get isEmpty():Boolean
		{
			for (var key:* in _queues) {
				return false;
			}
			if (_lazyArray.length > 0) {
				_min = 0x7FFFFFFF;
				useLazyArray();
				return false;
			}
			return true;
		}
		
		public function get length():int
		{
			var ret:int = 0;
			for (var k:* in _queues) {
				++ret;
			}
			return ret;
		}
		
		public function clear():void
		{
			for (var k:* in _queues) {
				delete _queues[k];
			}
			_min = 0x7FFFFFFF;
		}
		
		protected function useLazyArray():void
		{
			var path:Path;
			//if (_lazyArray.length > 0)
			//	clear();
			while (null != (path = _lazyArray.shift())) {
				// reinsert all retained data to the processing buffers
				push(path);
			}
		}
		
		/**
		 * The path will be processed after all the paths of the current Cost.
		 */
		[Inline]
		public final function pushLazy(path:Path):void
		{
			_lazyArray.push(path);
		}
		
		/**
		 * The path is inserted in the processing list. 
		 * If its cost is lower than current level it will be executed the next. 
		 */
		public function push(path:Path):void
		{
			var icost:int = 10000000.0 * path.cost;
			var queue:Array = _queues[icost];
			if (queue == null) {
				queue = []; 
				_queues[icost] = queue;
				if (_min > icost)
					_min = icost;
			}
			queue.push(path);
		}
		
		/**
		 * Retrieves the next path stored with the lower cost.
		 */
		public function pop():Path
		{
			if (isEmpty) 
				return null;
			
			// Takes the minimum cost queue 
			var min:int = _min;
			var queue:Array = _queues[min];
			var value:Path = queue.shift();
			if (value == null) {
				throw new Error("[PrioritizedQueue] pop error");
			}
			if (queue.length == 0) {
				// Use the lazy array when current has finished
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

		public function get currentPriority():int
		{
			return _min;
		}

	}	
}
