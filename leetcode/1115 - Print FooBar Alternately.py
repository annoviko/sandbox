class FooBar:
    def __init__(self, n):
        self.n = n
        self.sem_foo = threading.Semaphore(0)
        self.sem_bar = threading.Semaphore(0)


    def foo(self, printFoo):
        
        for i in range(self.n):
            
            # printFoo() outputs "foo". Do not change or remove this line.
            printFoo()

            self.sem_foo.release()   # increment foo
            self.sem_bar.acquire()   # wait when bar is non zero


    def bar(self, printBar: 'Callable[[], None]') -> None:
        
        for i in range(self.n):
            self.sem_foo.acquire()   # wait when it is not 0 and decrement

            # printBar() outputs "bar". Do not change or remove this line.
            printBar()

            self.sem_bar.release()   # increment bar