#include "fractol.h"

int	key_handler(int keycode, void *param)
{
	t_mlx *app;

	app = (t_mlx *) param;
	//printf("\x1b[1F\r\x1b[0K%d\n", keycode);
	if (keycode == 53)
		destroy_win(app);
	else if (keycode == 25)
		call_item(app, in_circle, 9);
	else if (keycode == 18)
		call_item(app, in_mandelbrot, 1);
	return (1);
}

int	mouse_handler(int button, int x, int y, void *param)
{
	t_mlx	*app;

	app = (t_mlx *) param;
	app->mouse_x = x;
	app->mouse_y = y;
	if (button == 1)
		printf("x: %4d\ty: %4d\n", x, y);
	else if (button == 4 || button == 5)
		zoom_handler(app, button, x, y);
	return (1);
}

int	zoom_handler(t_mlx *app, int button, int x, int y)
{
	int	color[] = {0x00FFFFFF, 0x00000000};

	if (app->mode == 0)
		return (0);
	if (button == 4 && app->border < 2147483597)
	{
		app->border += 50;
		app->offset_x += sign(app->size_x / 2 - x) * 40;
		app->offset_y += sign(app->size_y / 2 - y) * 40;
	}
	else if (app->border > 50)
	{
		app->border += 50;
		//app->offset_x -= sign(app->size_x / 2 + app->offset_x - x) * 0.25;
		//app->offset_y -= sign(app->size_y / 2 + app->offset_y - y) * 0.25;
	}
	if (app->mode == 9)
		draw_item(app, in_circle, colors);
	else if (app->mode == 1)
		draw_item(app, in_mandelbrot, colors);
	return (1);
}

int	put_frame(void *param)
{
	t_mlx	*app;

	app = (t_mlx *) param;
	if (app->is_modified)
	{
		mlx_put_image_to_window(app->mlx, app->win, app->frame->ptr, 0, 0);
		app->is_modified--;
	}
	return (1);
}

int	activate_frame(void *param)
{
	((t_mlx *) param)->is_modified = 100;
	return (1);
}
