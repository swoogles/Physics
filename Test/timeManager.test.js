const TimeManager = require('../src/timeManager');

describe('TimeManager', () => {
  let timeManager;

  beforeEach(() => {
    timeManager = new TimeManager();
  });

  test('should initialize with zero time', () => {
    expect(timeManager.getCurrentTime()).toBe(0);
  });

  test('should increment time correctly', (done) => {
    const callback = jest.fn();
    timeManager.addCallback(callback);
    timeManager.setTimeInterval(100);
    timeManager.start();
    
    setTimeout(() => {
      timeManager.stop();
      expect(timeManager.getCurrentTime()).toBe(100);
      expect(callback).toHaveBeenCalled();
      done();
    }, 150);
  });

  test('should handle multiple callbacks', (done) => {
    const callback1 = jest.fn();
    const callback2 = jest.fn();
    
    timeManager.addCallback(callback1);
    timeManager.addCallback(callback2);
    timeManager.setTimeInterval(100);
    timeManager.start();
    
    setTimeout(() => {
      timeManager.stop();
      expect(callback1).toHaveBeenCalled();
      expect(callback2).toHaveBeenCalled();
      done();
    }, 150);
  });
});