from subprocess import check_output, Popen, PIPE
from Generate import Generator
from os.path import join, dirname, abspath, exists, isfile
from os import makedirs, remove, devnull
import sys
import difflib
import time

class TestClass:
    """Compare json matches and measure performance

    """
    SUC = '\033[92m'
    FAIL = '\033[91m'
    OK = '\033[90m' 
    END = '\033[0m'

    def __init__(self, categories, gendict):
        self.categories = Generator.get_categories()
        self.words_path = join(dirname(dirname(abspath(__file__))), 'words.txt')
        self.ref_dict_path = join(dirname(dirname(abspath(__file__))), 'ref/Dict.bin')
        self.ref_compiler_path = join(dirname(dirname(abspath(__file__))), 'ref/TextMiningCompiler')
        self.ref_app_path = join(dirname(dirname(abspath(__file__))), 'ref/TextMiningApp')
        self.bin_dict_path = join(dirname(dirname(abspath(__file__))), 'build/Dict.bin')
        self.bin_app_path = join(dirname(dirname(abspath(__file__))), 'build/TextMiningApp')
        self.bin_compiler_path = join(dirname(dirname(abspath(__file__))), 'build/TextMiningCompiler')
        self.out_path = join(dirname(abspath(__file__)), 'output')
        self.in_path = join(dirname(abspath(__file__)), 'input')

        if gendict:
            self.generate_dicts()
        self.test(categories)

    def generate_dicts(self):
        dev_null = open(devnull, 'w')
        Popen([self.ref_compiler_path,  self.words_path, self.ref_dict_path], stdout=dev_null)
        Popen([self.bin_compiler_path, self.words_path, self.bin_dict_path], stdout=dev_null)
        dev_null.close()

    def diff_output(self):
        print "diff"

    def test(self, categories):
        print "\nTesting Examples"
        if not exists(self.out_path):
            makedirs(self.out_path)
        cats = categories if categories else self.categories
        index = 1
        for category in cats:
            sys.stdout.write("\r >> Testing Category " + str(index) + " out of " + str(len(cats)))
            sys.stdout.flush()
            if category not in self.categories:
                print "Category not implemented: " + category
                print self.categories
                exit(1)
            dev_null = open(devnull, 'w')
            cat = Popen(('cat', join(self.in_path, category)), stdout=PIPE)
            t1 = time.time()
            p1 = Popen([self.ref_app_path, self.ref_dict_path], stdin=cat.stdout, stdout=PIPE, stderr=dev_null)
            t2 = time.time()
            ref_out_json = p1.communicate()[0]

            cat = Popen(('cat', join(self.in_path, category)), stdout=PIPE)
            t3 = time.time()
            p2 = Popen([self.bin_app_path, self.bin_dict_path], stdin=cat.stdout, stdout=PIPE, stderr=dev_null)
            t4 = time.time()
            bin_out_json = p2.communicate()[0]
            dev_null.close()

            diff = difflib.ndiff(ref_out_json, bin_out_json)
            if diff:
                print '\r' + '[' + str(index) + '] ' + TestClass.FAIL + "FAILED - Output Difference - " + TestClass.END + category
            else:
                if t4 - t3 > t2 - t1:
                    print '\r' + '[' + str(index) + '] ' + TestClass.SUC + "SUCCESS - " + TestClass.END + category
                else:
                    print '\r' + '[' + str(index) + '] ' + TestClass.OK + "OK - Performance - " + TestClass.END + category

            outpath = join(self.out_path, category)
            if isfile(outpath + '_bin'):
                remove(outpath + '_bin')
            if isfile(outpath + '_ref'):
                remove(outpath + '_ref')
            bin_out = open(join(self.out_path, category + '_bin'), 'w')
            bin_out.write(bin_out_json)
            bin_out.close()
            ref_out = open(join(self.out_path, category + '_ref'), 'w')
            ref_out.write(ref_out_json)
            ref_out.close()

            index += 1


