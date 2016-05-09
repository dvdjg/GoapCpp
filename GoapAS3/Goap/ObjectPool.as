package Goap
{
	import flash.utils.getDefinitionByName;
	import flash.utils.getQualifiedClassName;
	/**
	 */
	public class ObjectPool
	{
		public static const POOL_SIZE:uint = 4096;
		private static var _instances:Object = new Object;
		private static var _reusedCount:uint = 0;
		private static var _newsCount:uint = 0;
		protected static var _poolObject:ObjectPool = getInstance(Object);

		protected var _objects:Array;
		protected var _objectClass:Class;
	
		public function ObjectPool(objectClass:Class)
		{
			//if(_instance){
			//	throw new Error("Singleton... use getInstance()");
			//} 
			//_instance = this;
			_objectClass = objectClass;
			_objects = new Array();
		}
		
		public function getPoolArray():Array
		{
			return _objects;
		}

		public static function get objectPoolInstance():ObjectPool
		{
			return _poolObject;
		}
		
		public static function getInstanceFromObject(obj:*):ObjectPool
		{
			return getInstanceFromName(getQualifiedClassName(obj));
		}
		
		public static function getInstance(objectClass:Class):ObjectPool
		{
			var qualifiedName:String = getQualifiedClassName(objectClass);
			if (!_instances[qualifiedName]) {
				_instances[qualifiedName] = new ObjectPool(objectClass);
			} 
			return _instances[qualifiedName];
		}

		private static function getInstanceFromName(qualifiedName:String):ObjectPool
		{
			if (!_instances[qualifiedName]) {
				_instances[qualifiedName] = new ObjectPool(Class(getDefinitionByName(qualifiedName)));
			} 
			return _instances[qualifiedName];
		}
		
		/** Releases the objects in the pool
		 */
		public static function releaseAllObjects():void
		{
			for (var className:String in _instances) {
				_instances[className].releaseObjects();
			}
			_reusedCount = 0;
			_newsCount = 0;
		}
	
//		public static function get countAllObjects():int
//		{
//			var ret:int = 0;
//			for (var m:String in _instances) {
//				if(_instances[m] is Object && _instances[m].hasOwnProperty("countObjects"))
//					ret += _instances[m].countObjects;
//			}
//			return ret;
//		}
		/**
		 * Call every object in the pool
		 */ 
		public function poolMap(callback:Function, arg:* = null):void
		{
			var nObj:int = _objects.length;
			for (var i:int = 0; i < nObj; ++i) {
				callback(_objects[i], arg);
			}
		}	
		
		public static function get reusedCount():uint
		{
			return _reusedCount;
		}
		
		public static function get newsCount():uint
		{
			return _newsCount;
		}
		
		public function getNew():*
		{
			var object:* = null;
			if (_objects.length > 0) {
				object = _objects.pop(); // LIFO
				++_reusedCount; // Count the number of times a new allocation has been avoided.
			} else {
				object = new _objectClass();
				++_newsCount;
			}
			return object;
		}
		
		public function getNew1Arg(arg1:*):*
		{
			var object:* = null;
			if (_objects.length > 0) {
				object = _objects.pop(); // LIFO
				++_reusedCount; // Count the number of times a new allocation has been avoided.
			} else {
				object = new _objectClass(arg1);
				++_newsCount;
			}
			return object;
		}	
		public static function reuse(oldObject:*, bDispose:Boolean = false):void
		{
			getInstanceFromName(getQualifiedClassName(oldObject)).dispose(oldObject, bDispose);
			//getInstance(getClass(oldObject)).reuseObject(oldObject, bDispose);
		}
		
		public function dispose(oldObject:*, bDispose:Boolean = false):void
		{
			if (oldObject != null) {
				if (bDispose)
					disposeObject(oldObject);
				if (_objects.length < POOL_SIZE)
					_objects.push(oldObject); // LIFO
			}
		}
		
		public function releaseObjects():void
		{
			_objects.length = 0;
		}
		
		public function get countObjects():int
		{
			return _objects.length;
		}
		
		public static function disposeObject(obj:*):*
        {
            if (obj == null)
				return obj;
            
            var i:int;
            var child:Object;
            
            var className:String = getQualifiedClassName(obj);
            if (className.indexOf("__AS3__.vec::Vector") == 0 || className == "Array")
            {
                var len:int = obj.length;
                
                for(i = 0; i < len; i++) {
                    reuse(obj[i], true);
                    obj[i] = null;
                }
                if(obj.fixed == false) obj.length = 0;
                return obj;
            }
            if (className == "flash.utils::Dictionary")
            {
                //Get Keys from dictionary.
                var idVec:Vector.<Object> = new Vector.<Object>(0);
                for(var subobj:Object in obj){
                    idVec.push(subobj);
                }
                
                //Delete Keys from dictionary and clear from vector at same time.
                var vLen:int = idVec.length;
                for (var vi:int = 0; vi<vLen; vi++) {
                    reuse( obj[ idVec[vi] ], true);
                    delete obj[ idVec[vi] ];
                    idVec[vi] = null;
                }
                
                idVec.length = 0;
                idVec = null;                
            }
                        
            if (obj.hasOwnProperty("numChildren") && obj.toString() != "[object TextField]")
            {
                while (obj.numChildren > 0) 
                {
                    child = obj.getChildAt(0);
                    if (child.hasOwnProperty("destroy")) 
                        disposeObject(child);
                    if (child.hasOwnProperty("parent") && child.parent != null) 
                        child.parent.removeChild(child);
					
                    reuse(child);
                    child = null;
                }
            }
            
            if (obj.hasOwnProperty("stop"))
                obj.stop();
            if (obj.hasOwnProperty("destroy")) 
                obj.destroy();
            if (obj.hasOwnProperty("bitmapData") && obj.bitmapData != null)
                obj.bitmapData.dispose();            
            if (obj.hasOwnProperty("parent") && obj.parent != null && obj.parent.hasOwnProperty("removeChild"))
                obj.parent.removeChild(obj);
            if (obj.hasOwnProperty("transform"))
                if(obj.transform != null) 
                    obj.transform.matrix = null;
            if (obj.hasOwnProperty("graphics")) 
                obj.graphics.clear();
            if (obj.hasOwnProperty("filters")) 
                obj.filters = [];
            if (obj.hasOwnProperty("dispose"))
                obj.dispose();
			if (obj.hasOwnProperty("clear"))
                obj.clear();

            return obj;
        }
		
		public static function getClass(obj:Object):Class {
			// If the object is not a Proxy subclass or the basic type int, it could be used:
			// var myClass:Class = Object(myObj).constructor;
			return Class(getDefinitionByName(getQualifiedClassName(obj)));
		}
	}
}