#! /usr/bin/python

import argparse
from Generate import Generator
from Test import TestClass


def parse_args():
    """Command Line argument parser
    The handled options are : 
    -g, generate new test examples
    -c, test named categories
    """
    parser = argparse.ArgumentParser(description='TextMiningApp Test Suite')
    parser.add_argument("-g", "--generate", help='Generate new test examples', dest='gen', default=False,
                        action='store_true')
    parser.add_argument("-c", "--categories", help='Test named categories', dest='cat', default=False,
                        action='store_true')
    parser.add_argument("-d", "--gen_dict", help='Generate Apps Dicts', dest='dic', default=False,
                        action='store_true')
    parser.add_argument("-p", "--print_categories", help="Print the names of categories", dest='pcat', default=False,
                        action='store_true')
    parser.add_argument("category", nargs='*')
    args = parser.parse_args()
    if args.pcat:
        print Generator.get_categories()
        exit(0)
    if not args.cat and args.category:
        print "Too much positionnal arguments"
        parser.print_help()
        exit(1)
    if args.gen: Generator(args.category, int(100))
    TestClass(args.category)


if __name__ == '__main__':
    parse_args()
