package Goap.Graph
{
	import Goap.IReadState;
	import Goap.IWriteState;
	import Goap.ObjectPool;
	import Goap.Planning.IPlanningAction;

	/**
	 * The path is a component of the search tree.
	 * The tree begins with a root Path element wich has no parent.
	 * Any other Path has one parent and 0 or more children.
	 * The Path holds an Action transforming its input State to an Output State. 
	 */
	public final class Path
	{
		public static var pool:ObjectPool = ObjectPool.getInstance(Path);
		
		public static function getNew(action:IPlanningAction, parent:Path = null, cost:Number = 0):Path
		{
			var path:Path = pool.getNew();
			path._action = action;
			path._parent = parent;
			path._cost = cost;
			path._initialState = null;
			path._finalState = null;
			return path;
		}
		
		private var _action:IPlanningAction;
		private var _cost:Number;
		private var _parent:Path;
		
		// Cache the map betwen a initial state and its final state
		private var _initialState:IReadState;
		private var _finalState:IWriteState;
		
		public function Path(action_:IPlanningAction=null, parent_:Path = null, cost_:Number = 0)
		{
			_action = action_;
			_parent = parent_;
			_cost = cost_;
		}

		[Inline]
		public final function dispose():void
		{
			_action = null;
			_parent = null;
			_cost = 1;
			_initialState = null;
			_finalState = null;
			pool.dispose(this);
		}

		[Inline]
		public final function get parent():Path
		{
			return _parent;
		}

		[Inline]
		public final function get cost():Number
		{
			return _cost;
		}

		[Inline]
		public final function get distance():Number
		{
			return _parent == null ? _cost : _cost - _parent.cost;
		}

		[Inline]
		public final function get action():IPlanningAction
		{
			return _action;
		}

		[Inline]
		public final function addChild(node:IPlanningAction, cost_:Number):Path
		{
			return getNew(node, this, _cost + cost_);
		}
		
		/**
		 * Returns the final state of the secuence of actions this path represents.
		 * This function is called very often when searching so we cache the result state. 
		 */
		public function executeFromRoot(initialState:IReadState):IWriteState
		{
			var finalState:IWriteState;
			if (_initialState && _initialState.equals(initialState)) {
				finalState = _finalState;
			} else {
				var state:IReadState = (_parent == null) ? initialState : _parent.executeFromRoot(initialState);
				finalState = _action.execute(state);
				_initialState = initialState;
				_finalState = finalState;
			}
			return finalState;
		}
		
		/**
		 * Returns the number of acctions of the path: from the root to this leave.
		 */
		public function getActionCount():int
		{
			var path:Path;
			var nActions:int = 0;
			for (path = this; path != null; path = path._parent) {
				nActions++;
			}
			return nActions;
		}
		
		/**
		 * Get the actions from the root to this leave.
		 * If the 'actions' parameter is given then accions will be unshift to the array.
		 */
		public function getActions(actions:Vector.<IPlanningAction> = null):Vector.<IPlanningAction>
		{
			if (actions == null)
				actions = new Vector.<IPlanningAction>;
			
			var path:Path;
			for (path = this; path != null; path = path._parent) {
				actions.unshift(path._action);
			}
			return actions;
		}
		
		/**
		 * Returns a list with the resulting states of the execution of the stored actions from the
		 * supplied initialState.
		 * This function caches the computed list of states so if it is used the same 'initialState'
		 * of a prior call, it will be returned the cached results with no need of executing the actions again.
		 */
		public function getStates(initialState:IReadState, states:Vector.<IReadState> = null):Vector.<IReadState>
		{
			if (states == null)
				states = new Vector.<IReadState>;
			
			var path:Path;
			for (path = this; path != null; path = path._parent) {
				states.unshift(path.executeFromRoot(initialState));
			}
			return states;
		}
	}	
}
