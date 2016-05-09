package Goap
{
	/**
	 * A world state
	 */
	public final class State implements IWriteState
	{
		public static var pool:ObjectPool = ObjectPool.getInstance(State);
		private var _map:Object = new Object;
		private var _hash:uint = 0;
		private var _cost:Number = 1.0;
		
		[Inline]
		public static function getNew(map:Object = null):State
		{
			var state:State = State.pool.getNew();
			if(map)
				state.map = map;
			else
				state.clearState();
			// state._hash = 0;
			state._cost = 1.0;
			return state;
		}
		
		public static function clearPoolStates():void
		{
			pool.poolMap(function(state:State, arg:*):void 
			{
				state._map = {};
				state._hash = 0;
				state._cost = 1.0;	
			});
		}
		
		/**
		 * Clone everything except the state cost.
		 */
		[Inline]
		public final function clone():IWriteState
		{
			var state:State = getNew(_map);
			state._hash = _hash;
			//state._cost = _cost;
			return state;
		}
		
		[Inline]
		public final function dispose():void
		{
			pool.dispose(this);
		}
		
		public function State(map:Object = null)
		{
			if (map) {
				this.map = map;
			}
		}
		
		[Inline]
		public final function resetState():void
		{
			_map = {};
			touch();
		}
		
		[Inline]
		public final function clearState():void
		{
			for (var key:String in _map) {
				_map[key] = null;
			}
			touch();
		}		
		/**
		 * Call this function when the state has changed
		 */
		[Inline]
		public final function touch():void
		{
			_hash = 0;
		}
		
		[Inline]
		public final function get map():Object
		{
			return _map;
		}

		[Inline]
		public final function getMap():Object
		{
			return _map;
		}

		[Inline]
		public final function set map(m:Object):void
		{
			setMap(m);
		}
		
		[Inline]
		public final function setMap(m:Object):void
		{
			for (var key:String in _map) {
				if (m[key] == null)
					_map[key] = null;
			}
			for (key in m) {
				var val:* = m[key];
				if (val != null)
					_map[key] = val;  // Assumes the values are not references
			}
			touch();
		}
		
		public function get length():int
		{
			var ret:int = 0;
			for each (var val:* in _map) {
				if (val != null)
					++ret; // Count only not null values
			}
			return ret;
		}
		
		[Inline]
		public final function put(key:String, value:*):IWriteState
		{
			_map[key] = value;
			touch();
			return this;
		}
		
		// Returned value must not be changed if is a reference type
		[Inline]
		public final function at(key:String):*
		{
			var ret:* = _map[key];
			return ret;
		}
		
		[Inline]
		public final function add(key:String, value:*):IWriteState
		{
			_map[key] += value;
			touch();
			return this;
		}
		
		[Inline]
		public final function mul(key:String, value:*):IWriteState
		{
			_map[key] *= value;
			touch();
			return this;
		}
		
		[Inline]
		public final function and(key:String, value:Boolean):IWriteState
		{
			_map[key] = _map[key] && value;
			touch();
			return this;
		}
		
		[Inline]
		public final function or(key:String, value:Boolean):IWriteState
		{
			_map[key] = _map[key] || value;
			touch();
			return this;
		}		
		[Inline]
		public final function remove(key:String):IWriteState
		{
			_map[key] = null;
			touch();
			return this;
		}
		
		[Inline]
		public final function equals(other:IReadState):Boolean
		{
			return hash == other.hash;
		}
		
		// This a member comparison equality
		public function memberEquals(other:IReadState):Boolean
		{
			var s1k:*;
			var s2k:*;
			var percent:Number = 0;
			var countDifferent:Number = 0;
			var s1Count:int = 0;
			var sameCount:int = 0;
			for (var key:String in this.map) {
				s1k = other.at(key);
				if (s1k == null) {
					return false;
				}
				s2k = this.at(key);
				if (s1k != s2k) {
					return false;
				}
				++s1Count;
			}
			return s1Count == other.length;
		}
		
		// Text representation of the object
		public function toString():String {
			var ret:String;
			ret = "{<cost>=" + _cost;
			for (var key:String in _map) {
				var val:* = _map[key];
				if (val != null) // Show only not null values
					ret += " " + key + "=" + val;
			}
			ret += "}";
			return ret;
		}

		[Inline]
		public final function get hash():uint
		{
			if (_hash == 0) {
				_hash = Hashing.hashObject(map);
			}
			return _hash;
		}

		/**
		 * The cost of executing the action that generated this state over a previous state.
		 * Te cost is not part of the state so it will not create different states.
		 */
		[Inline]
		public final function get cost():Number
		{
			return _cost;
		}
		
		[Inline]
		public final function getCost():Number
		{
			return _cost;
		}

		[Inline]
		public final function set cost(value:Number):void
		{
			_cost = value;
		}

		[Inline]
		public final function setCost(value:Number):IWriteState
		{
			_cost = value;
			return this;
		}

		[Inline]
		public final function addCost(value:Number):IWriteState
		{
			_cost += value;
			return this;
		}
		
		[Inline]
		public final function mulCost(value:Number):IWriteState
		{
			_cost *= value;
			return this;
		}
	
	}	
}
