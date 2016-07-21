import inspect
import random
from os.path import join,dirname, abspath, isfile, exists
from os import makedirs, remove
import string
import sys


class Generator:
    """Generate batches of test examples

    The tests are generated randomly in multiple categories to
    ensure most cases are covered.

    """
    def __init__(self, categories, num_tests):

        self.dict_path = join(dirname(dirname(abspath(__file__))), 'words.txt')
        self.path = join(dirname(abspath(__file__)), 'input')
        self.num_tests = num_tests
        self.categories = Generator.get_categories()
        self.words = []
        self.generate(categories)

    def generate(self, categories=None):
        print "Test Examples Generation"
        self.select_from_dict()
        if not exists(self.path):
            makedirs(self.path)
        cats = categories if categories else self.categories
        index = 1
        for category in cats:
            sys.stdout.write("\r\t >> Generating Category " + str(index) + " out  of " + str(len(cats)))
            sys.stdout.flush()
            if category not in self.categories:
                print "Category not implemented: " + category
                print self.categories
                exit(1)
            call = getattr(Generator, category)
            call(self)
            index += 1

    @staticmethod
    def get_categories():
        rem = ['__doc__', '__module__',
               'generate', '__init__',
               'get_categories', 'select_from_dict',
               'rand_words_n']
        return [x for x in [n for n, v in inspect.getmembers(Generator, None)] if x not in rem]

    def select_from_dict(self):
        if not isfile(self.dict_path):
            print "Dict file does not exist"
            exit(1)
        print "Selecting Random Words from dictionary ..."
        f = open(self.dict_path, 'r')
        num_lines = 3134935
        indices = random.sample(range(1, num_lines), self.num_tests)
        for i, line in enumerate(f):
            if i in indices:
                indices.remove(i)
                self.words.append(line.split()[0])

    def swap_rand(self):
        path = join(self.path, 'swap_rand')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            index = random.randint(1, len(word) - 1)
            str = word[:index - 1] + word[index] + word[index - 1] + word[index + 1:] + '\n'
            f.write("approx 1 " + str)
        f.close()

    def ins_rand(self):
        path = join(self.path, 'ins_rand')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            index = random.randint(1, len(word) - 1)
            chr = random.choice(string.letters + string.digits + string.punctuation)
            str = word[:index - 1] + chr +  word[index - 1:] + '\n'
            f.write("approx 1 " + str)
        f.close()

    def del_rand(self):
        path = join(self.path, 'del_rand')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            index = random.randint(1, len(word) - 1)
            str = word[:index - 1] + word[index:] + '\n'
            f.write("approx 1 " + str)
        f.close()

    def swap_first(self):
        path = join(self.path, 'swap_first')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            index = 1
            str = word[:index - 1] + word[index] + word[index - 1] + word[index + 1:] + '\n'
            f.write("approx 1 " + str)
        f.close()

    def swap_last(self):
        path = join(self.path, 'swap_last')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            index = 1
            str = word[:-2] + word[-1] + word[-2] + '\n'
            f.write("approx 1 " + str)
        f.close()

    def del_first(self):
        path = join(self.path, 'del_first')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            str = word[1:] + '\n'
            f.write("approx 1 " + str)
        f.close()

    def del_last(self):
        path = join(self.path, 'del_last')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            str = word[:-1] + '\n'
            f.write("approx 1 " + str)
        f.close()

    def ins_first(self):
        path = join(self.path, 'ins_first')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            chr = random.choice(string.letters + string.digits + string.punctuation)
            str = chr + word + '\n'
            f.write("approx 1 " + str)
        f.close()

    def ins_last(self):
        path = join(self.path, 'ins_last')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            index = random.randint(1, len(word) - 1)
            chr = random.choice(string.letters + string.digits + string.punctuation)
            str = word + chr + '\n'
            f.write("approx 1 " + str)
        f.close()

    def swap_two_first(self):
        path = join(self.path, 'swap_two_first')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            if len(word) > 3:
                str = word[2] + word[1] + word[0] + word[3:] + '\n'
                f.write("approx 2 " + str)
        f.close()

    def swap_two_last(self):
        path = join(self.path, 'swap_two_last')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            if len(word) > 3:
                str = word[:-3] + word[-1] + word[-2] + word[-3] + '\n'
                f.write("approx 2 " + str)
        f.close()

    def del_two_first(self):
        path = join(self.path, 'del_two_first')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            if len(word) > 3:
                str = word[2:] + '\n'
                f.write("approx 2 " + str)
        f.close()

    def del_two_last(self):
        path = join(self.path, 'del_two_last')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            if len(word) > 3:
                str = word[:-2] + '\n'
                f.write("approx 2 " + str)
        f.close()

    def ins_two_first(self):
        path = join(self.path, 'ins_two_first')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            chr = random.choice(string.letters + string.digits + string.punctuation)
            chr1 = random.choice(string.letters + string.digits + string.punctuation)
            str = chr + chr1 + word + '\n'
            f.write("approx 2 " + str)
        f.close()

    def ins_two_last(self):
        path = join(self.path, 'ins_two_last')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            index = random.randint(1, len(word) - 1)
            chr = random.choice(string.letters + string.digits + string.punctuation)
            chr1 = random.choice(string.letters + string.digits + string.punctuation)
            str = word + chr + chr1 + '\n'
            f.write("approx 2 " + str)
        f.close()

    def rand_words_n(self, n):
        path = join(self.path, 'rand_words' + str(n))
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for i in range(len(self.words)):
            length = random.randint(1, 20)
            strg = ''.join(random.sample(string.letters + string.digits + string.punctuation, length)) + '\n'
            f.write("approx " + str(n) + " " + strg)
        f.close()

    def rand_words1(self):
        self.rand_words_n(1)

    def rand_words2(self):
        self.rand_words_n(2)

    def rand_words3(self):
        self.rand_words_n(3)

    def dist0(self):
        path = join(self.path, 'dist0')
        if isfile(path):
            remove(path)
        f = open(path, 'w+')
        for word in self.words:
            str = word + '\n'
            f.write("approx 0 " + str)
        f.close()
