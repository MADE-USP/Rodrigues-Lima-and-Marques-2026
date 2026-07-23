/******************************************************************************

	SUPPORT C FUNCTIONS
	-------------------

	Written by Lucca Rodrigues, Made-USP

	Pure C support functions used in the objects in the Green Made model are
	coded below.

 ******************************************************************************/

/*============================== SUPPORT MACROS ==============================*/

// send machine brochure to consumption-good client firm in equations '_new_clientsK',
// '_supplier'

object *send_brochure( object *suppl, object *client )
{
	object *broch, *cli;

	cli = ADDOBJS( suppl, "clients" );				// add object to new client
	WRITES( cli, "__IDc", VS( client, "_id_firm_c" ) );// update client ID
	WRITES( cli, "__tSel", T );					// update selection time

	broch = ADDOBJS( client, "broch" );			// add brochure to client
	WRITES( broch, "__IDs", VS( suppl, "_id_firm_k" ) );// update supplier ID
	WRITE_SHOOKS( broch, cli );					// pointer to supplier client list
	WRITE_SHOOKS( cli, broch );					// pointer to client brochure list

	return broch;
}

object *set_supplier( object *firm )
{
	object *broch, *suppl,
		   *cap = V_EXTS( GRANDPARENTS( firm ), countryE, k_sector );

	suppl = RNDDRAWS( cap, "k_sector", "_productivity_k" );// draw capital supplier
	broch = send_brochure( suppl, firm );		// get supplier brochure
	WRITE_SHOOKS( firm, broch );			// pointer to current supplier
	INCRS( suppl, "_clients_new_k", 1 );					// update supplier's clients #

	return suppl;
}

void send_order( object *firm, double nMach )
{
	// find firm entry on supplier client list
	object *cli = SHOOKS( SHOOKS( firm ) );

	if ( VS( cli, "__tOrd" ) < T )				// if first order in period
	{
		WRITES( cli, "__nOrd", nMach );			// set new order size
		WRITES( cli, "__tOrd", T );				// set order -c
		WRITES( cli, "__nCan", 0 );				// no machine canceled yet
	}
	else
		INCRS( cli, "__nOrd", nMach );			// increase existing order size
}


const char *CliBrochObj[ ] = { "clients", "broch" };

