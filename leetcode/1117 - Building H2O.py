class H2O:
    def __init__(self):
        self.sem_o = threading.Semaphore(1)
        self.sem_h = threading.Semaphore(0)

        self.mutex_h = threading.Lock()
        self.counter_h = 0


    def hydrogen(self, releaseHydrogen: 'Callable[[], None]') -> None:
        self.sem_h.acquire()

        # releaseHydrogen() outputs "H". Do not change or remove this line.
        releaseHydrogen()

        with self.mutex_h:
            self.counter_h += 1

            if self.counter_h % 2 == 0:
                self.sem_o.release()


    def oxygen(self, releaseOxygen: 'Callable[[], None]') -> None:
        self.sem_o.acquire()

        # releaseOxygen() outputs "O". Do not change or remove this line.
        releaseOxygen()

        self.sem_h.release(2)
