template <Unit>
struct Image
{
	template<size_t i>
	Image(Unit(&raw_image)[i], size_t width, size_t height)
	:raw_image(raw_image), width(width), height(height)
	{}

private:
	size_t width;
	size_t height;
	size_t size;
	Unit const * raw_image;
}