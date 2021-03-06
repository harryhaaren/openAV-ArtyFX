
#include "kuiza.hxx"
#include "../dsp/kuiza.hxx"

#include "common.hxx"
#include "../avtk/theme.hxx"
#include "graphs/eq.hxx"
#include "headers/kuiza.c"

KuizaUI::KuizaUI(PuglNativeWindow parent) :
	Avtk::UI( WIDTH, HEIGHT, parent, "Kuiza (ArtyFX-OpenAV)" )
{
	Avtk::Image* i = new Avtk::Image( this, 0, 0, 160,  29, "header");
	i->load( kuiza.pixel_data );

	graph = new Avtk::Eq( this, 5,36, 150, 126, "graph" );

	dial1 = new Avtk::Dial( this,  2, 166, 40,26, "Low"  );
	dial2 = new Avtk::Dial( this, 29, 185, 40,26, "Mid"  );
	dial3 = new Avtk::Dial( this, 58, 166, 40,26, "Mid"  );
	dial4 = new Avtk::Dial( this, 90, 185, 40,26, "High" );
	dial5 = new Avtk::Dial( this,119, 168, 40,36, "Gain" );

	dial1->defaultValue( 0.5 );
	dial2->defaultValue( 0.5 );
	dial3->defaultValue( 0.5 );
	dial4->defaultValue( 0.5 );
	dial5->defaultValue( 0.5 );
}

void KuizaUI::widgetValueCB( Avtk::Widget* widget )
{
	float v = widget->value();
	//printf("Widget %s : %f\n", widget->label(), v );
	if( widget == dial1 ) {
		graph->gains[0] = v;
		write_function( controller, KUIZA_GAIN_S1, sizeof(float), 0, &v );
	}
	if( widget == dial2 ) {
		graph->gains[1] = v;
		write_function( controller, KUIZA_GAIN_S2, sizeof(float), 0, &v );
	}
	if( widget == dial3 ) {
		graph->gains[2] = v;
		write_function( controller, KUIZA_GAIN_S3, sizeof(float), 0, &v );
	}
	if( widget == dial4 ) {
		graph->gains[3] = v;
		write_function( controller, KUIZA_GAIN_S4, sizeof(float), 0, &v );
	}
	if( widget == dial5 ) {
		graph->value( v );
		write_function( controller, KUIZA_GAIN, sizeof(float), 0, &v );
	}
	redraw();
}

void KuizaUI::lv2PortEvent( uint32_t index,
                            uint32_t buf_size,
                            uint32_t format,
                            const void* buffer )

{
	if( format != 0 )
		return;

	float v = *((float*)buffer);

	//printf("KuizaUI port() %i : v\n", index, v );

	switch( index ) {
	case KUIZA_GAIN_S1:
		dial1->value( v );
		graph->gains[0] = v;
		break;
	case KUIZA_GAIN_S2:
		dial2->value( v );
		graph->gains[1] = v;
		break;
	case KUIZA_GAIN_S3:
		dial3->value( v );
		graph->gains[2] = v;
		break;
	case KUIZA_GAIN_S4:
		dial4->value( v );
		graph->gains[3] = v;
		break;
	case KUIZA_GAIN:
		dial5->value( v );
		graph->value( v );
		break;
	}
	redraw();
	return;
}

