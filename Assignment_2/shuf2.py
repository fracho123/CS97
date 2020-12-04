#!/usr/bin/python

import random, sys, string
import argparse


class randline:
    def __init__(self, filename, file):
        if file == True:
            with open(filename) as f:
                self.lines = f.read().splitlines()
        else:
            self.lines=filename

    #when repeat is true
    def chooseline(self):
        return random.choice(self.lines)

    #when repeat is false
    def sample(self, count):
        return random.sample(self.lines, count)

def main():
    # messages
    usage_msg = """shuf [OPTION]... [FILE]
  or:  shuf -e [OPTION]... [ARG]...
  or:  shuf -i LO-HI [OPTION]...
Write a random permutation of the input lines to standard output."""
    desc_msg = 'With no FILE, or when FILE is -, read standard input.'
    group_msg = 'Mandatory arguments to long options are mandatory for short options too.'

    # help
    echo_help = 'treat each ARG as an input line'
    input_help = 'treat each number LO through HI as an input line'
    count_help = 'output at most COUNT lines'
    rep_help = 'output lines can be repeated'

    parser = argparse.ArgumentParser(prog='shuf', description=desc_msg, usage=usage_msg)
    group = parser.add_argument_group(group_msg)
    group.add_argument('-e', '--echo', nargs='*', default='null', help=echo_help)
    group.add_argument('-i', '--input-range=LO-HI', nargs='?', dest='limits', const="", default='null', help=input_help)
    group.add_argument('-n', '--head-count=COUNT', nargs='?', dest='count', const="", default=sys.maxsize, help=count_help)
    group.add_argument('-r', '--repeat', action='store_true', default=False, help=rep_help)
    #group.add_argument('elements', nargs='*', default='null')
    #group.add_argument('infile', nargs='?', const='null',type=argparse.FileType('r'), default=sys.stdin)


    options = args = parser.parse_known_args(sys.argv[1:])

    #check the argument after -n
    if options[0].count=='':
        parser.error("option requires an argument -- 'n'")
    try:
        count = int(options[0].count)
    except:
        parser.error("invalid line count: '{0}'".format(options[0].count))
    if count < 0:
        parser.error("invalid line count: '{0}'".format(count))

    input_file=False
    needfile = True
    hyphen = False #stdin if True
    e = options[0].echo
    r = options[0].repeat
    #infile = options[0].infile
    elements = options[1]
    limits = options[0].limits

    if e == 'null':
        for obj in elements:
            if obj == '-':
                hyphen = True
                #potentially stdin.


    #check the argument after -i
    if limits != 'null':
        if limits == '':
            parser.error("option requires an argument -- 'i'")
        limits = str(limits)
        try:
            lim = limits.split('-')
            low = int(lim[0])
            high = int(lim[1])
        except:
            parser.error("invalid input range: {0}".format(limits))
        if len(lim) != 2:
            parser.error("invalid input range: {0}".format(limits))
        #after this point, limits is deemed valid
        if e!='null': #this means -e was declared
            parser.error("cannot combine -e and -i options")

        if elements != []:
            parser.error("extra operand '{0}'".format(elements[0]))
        if low > high:
            return
        values=[]
        for x in range(low,high+1):
            val = str(x)#+'\n'
            values.append(val)
        generator = randline(values, False)
        needfile=False

    #formatting the -e input list
    if e != 'null':
        e=e+elements
        elements=[]
        generator = randline(e, False)
        if needfile == True:
            needfile = False


    #edited_f = []
   # f = open(unknown[0], "r").readlines()
    ## Creates list called f which holds the lines from the file
   # for line in f:
        #edited_f.append(line.rstrip('\n'))
################################################################
    length=len(elements)
    if needfile == True:
        if length == 0:
            inputs = []
            read_in = sys.stdin.readline().strip('\n')
            while read_in != "":
                inputs.append(read_in)
                read_in = sys.stdin.readline()
                if read_in == '\n':
                    continue
                else:
                    read_in = read_in.strip('\n')
            generator = randline(inputs, False)
            input_file = True
            if inputs == []:
                for line in sys.stdin:
                    ()
        elif length == 1:
            if hyphen == True:
                inputs=[]
                read_in = sys.stdin.readline().strip('\n')
                while read_in!="":
                    inputs.append(read_in)
                    read_in = sys.stdin.readline()
                    if read_in == '\n':
                        continue
                    else:
                        read_in = read_in.strip('\n')
                generator = randline(inputs, False)
                input_file=True
                if inputs == []:
                    for line in sys.stdin:
                        ()

            else:
                potential_file=elements[0]
                try:
                    generator = randline(potential_file, True)
                    input_file = True
                except:
                    #potential_file is not a file
                    parser.error("{0}: No such file or directory".format(elements[0]))
        elif length >= 2:
            parser.error("extra operand '{0}'".format(elements[1]))


    # if shuf - <test.txt = same as stdin
    if (r==True) and (len(generator.lines)==0):
        parser.error("no lines to repeat")

    if (e!='null') or (limits!='null') or (hyphen==True) or (input_file==True):
        if r == True:
            if count==sys.maxsize:
                while (r==True):
                    print(generator.chooseline())
            for index in range(count):
                if index=='\n':
                    print()
                else:
                    print(generator.chooseline())
        if r == False:
            if count>len(generator.lines):
                count=len(generator.lines)
            shuffled = generator.sample(count)
            length=len(shuffled)
            for obj in shuffled:
                #if j != 0 and (e!='clear' or limits != False):
                    #sys.stdout.write('\n')
                if obj=='\n':
                    print()
                else:
                    print(obj)

if __name__ == "__main__":
    main()