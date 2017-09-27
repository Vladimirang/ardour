#include <iostream>
#include <cstdlib>
#include <getopt.h>

#include "common.h"

using namespace std;
using namespace ARDOUR;
using namespace SessionUtils;


static void usage (int status)
{
	// help2man compatible format (standard GNU help-text)
	printf (UTILNAME " - create a new empty session from the commandline.\n\n");
	printf ("Usage: " UTILNAME " [ OPTIONS ] <session-dir> <session/snapshot-name>\n\n");
	printf ("Options:\n\
  -h, --help                 display this help and exit\n\
  -s, --samplerate <rate>    samplerate to use (default 48000)\n\
  -V, --version              print version information and exit\n\
\n");
	printf ("\n\
This tool creates a new empty Ardour session.\n\
\n\
Note: the tool expects a session-name without .ardour file-name extension.\n\
\n\
Examples:\n\
"UTILNAME " -s 44100 /tmp/TestSession TestSession\n\
\n");

	printf ("Report bugs to <http://tracker.ardour.org/>\n"
	        "Website: <http://ardour.org/>\n");
	::exit (status);
}

int main (int argc, char* argv[])
{
	int sample_rate = 48000;

	const char *optstring = "hs:V";

	const struct option longopts[] = {
		{ "help",       0, 0, 'h' },
		{ "samplerate", 1, 0, 's' },
		{ "version",    0, 0, 'V' },
	};

	int c = 0;
	while (EOF != (c = getopt_long (argc, argv,
					optstring, longopts, (int *) 0))) {
		switch (c) {
			case 's':
				{
					const int sr = atoi (optarg);
					if (sr >= 8000 && sr <= 192000) {
						sample_rate = sr;
					} else {
						fprintf(stderr, "Invalid Samplerate\n");
					}
				}
				break;

			case 'V':
				printf ("ardour-utils version %s\n\n", VERSIONSTRING);
				printf ("Copyright (C) GPL 2017 Robin Gareus <robin@gareus.org>\n");
				exit (0);
				break;

			case 'h':
				usage (0);
				break;

			default:
				usage (EXIT_FAILURE);
				break;
		}
	}

	// XXX perhaps allow to infer the session-name from the dir-name
	if (optind + 2 > argc) {
		usage (EXIT_FAILURE);
	}

	/* all systems go */

	SessionUtils::init();
	Session* s = 0;

	s = SessionUtils::create_session (argv[optind], argv[optind+1], sample_rate);

	/* save is implicit when creating a new session */

	SessionUtils::unload_session(s);
	SessionUtils::cleanup();

	return 0;
}
