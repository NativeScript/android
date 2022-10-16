global.zonedCallback = function (callback) {
			if (global.zone) {
				// Zone v0.5.* style callback wrapping
				return global.zone.bind(callback);
			}
			if (global.Zone) {
				// Zone v0.6.* style callback wrapping
				return global.Zone.current.wrap(callback);
			} else {
				return callback;
			}
		};


class FPSCallback {
	constructor(onFrame) {

        this.impl = null
        this.onFrame
        this.running = false
        this.sdkVersion = 0
        this.nativeFramesSupported = false
		this.running = false;
		this.onFrame = onFrame;

		this.sdkVersion = parseInt(android.os.Build.VERSION.SDK);
		this.nativeFramesSupported = this.sdkVersion >= 24 && this._isNativeFramesSupported();

		if (this.nativeFramesSupported) {
			this.impl = (nanos) => {
				this.handleFrame(nanos);
			};

			console.log('impl', this.impl);
		} else {
			this.impl = new android.view.Choreographer.FrameCallback({
				doFrame: (nanos) => {
					this.handleFrame(nanos);
				},
			});
		}
	}

	_isNativeFramesSupported() {
		return typeof global.__postFrameCallback === 'function' && typeof global.__removeFrameCallback === 'function';
	}

	start() {
		if (this.running) {
			return;
		}

		if (this.nativeFramesSupported) {
			global.__postFrameCallback(this.impl);
		} else {
			android.view.Choreographer.getInstance().postFrameCallback(this.impl);
		}

		this.running = true;
	}

	stop() {
		if (!this.running) {
			return;
		}

		if (this.nativeFramesSupported) {
			global.__removeFrameCallback(this.impl);
		} else {
			android.view.Choreographer.getInstance().removeFrameCallback(this.impl);
		}

		this.running = false;
	}

	handleFrame(nanos) {
		if (!this.running) {
			return;
		}

		// divide by 1 000 000 since the parameter is in nanoseconds
		this.onFrame(nanos / 1000000);
		// add the FrameCallback instance again since it is automatically removed from the Choreographer

		if (this.nativeFramesSupported) {
			global.__postFrameCallback(this.impl);
		} else {
			android.view.Choreographer.getInstance().postFrameCallback(this.impl);
		}
	}
}


function getTimeInFrameBase() {
	return java.lang.System.nanoTime() / 1000000;
}


function dispatchToMainThread(func) {
	const runOnMainThread = global.__runOnMainThread;
	if (runOnMainThread) {
		runOnMainThread(() => {
			func();
		});
	} else {
		new android.os.Handler(android.os.Looper.getMainLooper()).post(
			new java.lang.Runnable({
				run: func,
			})
		);
	}
}


let scheduled = false;
let macroTaskQueue = [];

function drainMacrotaskQueue() {
	const currentQueue = macroTaskQueue;
	macroTaskQueue = [];
	scheduled = false;
	currentQueue.forEach((task) => {
		try {
			task();
		} catch (err) {
			const msg = err ? err.stack || err : err;
		}
	});
}

function queueMacrotask(task) {
	macroTaskQueue.push(task);
	if (!scheduled) {
		scheduled = true;
		dispatchToMainThread(drainMacrotaskQueue);
	}
}

let animationId = 0;
let currentFrameAnimationCallbacks = {}; // requests that were scheduled in this frame and must be called ASAP
let currentFrameScheduled = false;
let nextFrameAnimationCallbacks = {}; // requests there were scheduled in another request and must be called in the next frame
let shouldStop = true;
let inAnimationFrame = false;
let fpsCallback;
let lastFrameTime = 0;

function getNewId() {
	return animationId++;
}

function ensureNative() {
	if (fpsCallback) {
		return;
	}
	fpsCallback = new FPSCallback(doFrame);
}

function callAnimationCallbacks(thisFrameCbs, frameTime) {
	inAnimationFrame = true;
	for (const animationId in thisFrameCbs) {
		if (thisFrameCbs[animationId]) {
			try {
				thisFrameCbs[animationId](frameTime);
			} catch (err) {
				const msg = err ? err.stack || err : err;
			}
		}
	}
	inAnimationFrame = false;
}

function doCurrentFrame() {
	// if we're not getting accurate frame times
	// set last frame time as the current time
	if (!fpsCallback || !fpsCallback.running) {
		lastFrameTime = getTimeInFrameBase();
	}
	currentFrameScheduled = false;
	const thisFrameCbs = currentFrameAnimationCallbacks;
	currentFrameAnimationCallbacks = {};
	callAnimationCallbacks(thisFrameCbs, lastFrameTime);
}

function doFrame(currentTimeMillis) {
	lastFrameTime = currentTimeMillis;
	shouldStop = true;
	const thisFrameCbs = nextFrameAnimationCallbacks;
	nextFrameAnimationCallbacks = {};
	callAnimationCallbacks(thisFrameCbs, lastFrameTime);
	if (shouldStop) {
		fpsCallback.stop(); // TODO: check performance without stopping to allow consistent frame times
	}
}

function ensureCurrentFrameScheduled() {
	if (!currentFrameScheduled) {
		currentFrameScheduled = true;
		queueMacrotask(doCurrentFrame);
	}
}

function requestAnimationFrame(cb) {
	const animId = getNewId();
	if (!inAnimationFrame) {
		ensureCurrentFrameScheduled();
		currentFrameAnimationCallbacks[animId] = zonedCallback(cb);
		return animId;
	}
	ensureNative();
	nextFrameAnimationCallbacks[animId] = zonedCallback(cb);
	shouldStop = false;
	fpsCallback.start();

	return animId;
}

function cancelAnimationFrame(id) {
	delete currentFrameAnimationCallbacks[id];
	delete nextFrameAnimationCallbacks[id];
}


module.exports = {
FPSCallback,
getTimeInFrameBase,
requestAnimationFrame,
cancelAnimationFrame,
queueMacrotask,
dispatchToMainThread
}