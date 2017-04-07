#!/usr/bin/python

import lldb
import getopt
import sys
import optparse
import shlex

def handle_options():
    usage = "Usage: %command [options]"
    description = 'This command uses two parameters: firstname and lastname.'
    parser = optparse.OptionParser(description=description, prog='insert_data', usage=usage)
    parser.add_option('-f', '--firstname', type='string', dest='firstname', help='Firstname.')
    parser.add_option('-l', '--lastname', type='string', dest='lastname', help='Lastname.')

    return parser

def insert_data(debugger, command, result, dict):
    firstname = 'John'
    lastname = 'Smith'

    command_args = shlex.split(command)
    parser = handle_options()

    # parse passed attributes
    try:
        (options, args) = parser.parse_args(command_args)
    except:
        result.SetError('Options parsing failed.')
        return

    if options.firstname:
        firstname = options.firstname
        print 'Firstname is: ' + firstname

    if options.lastname:
        lastname = options.lastname
        print 'Lastname is: ' + lastname

    # fill in fields with firstname and lastname
    
    # Objective-C expression
    cmd = 'expr (void) [TestClass handleFirstname:@"' + firstname + '"' + ' lastname:@"' + lastname + '"]'
    
    # C++/JNI expression
    # cmd = 'expr (void) TestClass::handleName' + '(' + '"' + firstname + '",' + '"' + lastname + '"' + ')'
    
    print cmd

    res = lldb.SBCommandReturnObject()
    lldb.debugger.GetCommandInterpreter().HandleCommand(cmd, res)

    # handle error if any
    if not res.Succeeded():
        print 'Error: Cannot execute command.'
        print res
        return

    # continue process execution
    target = lldb.debugger.GetSelectedTarget()
    process = target.GetProcess()
    lldb.debugger.SetAsync(True)
    process.Continue()

def __lldb_init_module(debugger, dict):
    # inject insert_data to be used during lldb session
    debugger.HandleCommand('command script add -f lldbutils.insert_data insert_data')

    # allows to print quite long string values
    debugger.HandleCommand('settings set target.max-string-summary-length 100000')